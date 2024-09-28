#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
#include <sstream>
#include <cstdint>

class _8086CPU {
private:
    std::vector<std::string> memory; // 存储指令和数据的内存模型
    std::map<std::string, int16_t> registers; // 寄存器组
    std::map<std::string, std::function<void(const std::string &, const std::string &)>> instructions; // 指令集及其对应的处理函数

    // 总线接口单元（BIU, Bus Interface Unit）
    struct BIU {
        _8086CPU* cpu; // 指向Simple8086实例的指针，以便访问寄存器和内存

        BIU(_8086CPU* _cpu) : cpu(_cpu) {}

        std::queue<std::pair<std::string, int>> instructionQueue; // 指令队列
        std::mutex queueMutex; // 互斥锁，用于保护指令队列

        std::map<std::string, int16_t> registers; // 段寄存器，指令指针，及内部寄存器

        std::map<std::string, std::string> addressBus; // 地址总线
        std::map<std::string, int16_t> dataBus; // 数据总线
        std::map<std::string, bool> controlBus; // 控制总线

        // 从内存中预取指令到指令队列
        void fetch() {
            int instructionNumber = cpu->memory.size(); // 指令条数
            while (true) {
                if (registers["IP"] >= instructionNumber) break; // 内存中的指令全部取出

                if (instructionQueue.empty()) { // 指令队列为空，预取下一条指令
                    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 模拟取指周期
                    std::unique_lock<std::mutex> lock(queueMutex); // 锁住指令队列，保证互斥使用
                    std::string instruction = cpu->memory[registers["IP"]];
                    instructionQueue.emplace(instruction, registers["IP"]); // 将指令预取出到指令队列
                    lock.unlock(); // 预取指令完成，解锁指令队列
                    // 提取操作码和操作数
                    std::istringstream iss(instruction);
                    std::string op;
                    iss >> op;
                    std::string opr;
                    iss >> opr;
                    if (op == "JMP") registers["IP"] = std::stoi(opr); // JMP则跳转地址
                    else registers["IP"]++; // 指令指针加1
                }
            }
        }

        // 更新总线状态
        void updateBuses(const std::string &operation, const std::string &registerName, const std::string &s) {
            // 清空总线状态
            addressBus.clear();
            dataBus.clear();
            controlBus.clear();

            // 根据操作更新总线
            if (operation == "MOV") { // 需判断是寄存器还是立即数
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) addressBus["source"] = s; // 找到是寄存器 传入地址总线
                else addressBus["source"] = "\\"; // 没找到寄存器 是立即数
                addressBus["destination"] = registerName;
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) dataBus["data"] = cpu->eu.registers[s]; // 找到是寄存器 传入数据总线
                else dataBus["data"] = std::stoi(s); // 字符串转int类型
                controlBus["read"] = true;
                controlBus["write"] = true;
            } else if (operation == "ADD") { // 需判断是寄存器还是立即数
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) addressBus["source"] = s;
                else addressBus["source"] = "\\";
                addressBus["destination"] = registerName;
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) dataBus["data"] = cpu->eu.registers[s];
                else dataBus["data"] = std::stoi(s);
                controlBus["read"] = true;
                controlBus["write"] = true;
            } else if (operation == "SUB") {
                addressBus["source"] = "\\";
                addressBus["destination"] = registerName;
                dataBus["data"] = std::stoi(s);
                controlBus["read"] = true;
                controlBus["write"] = true;
            } else if (operation == "MUL" || operation == "DIV") {
                addressBus["source"] = registerName;
                addressBus["destination"] = "AX";
                dataBus["data"] = std::stoi(s);
                controlBus["read"] = true;
                controlBus["write"] = true;
            }

            // 调试输出总线状态
            printBusState();
        }

        // 打印总线状态
        void printBusState() {
            std::cout << "地址总线:" << std::endl;
            for (const auto &[key, val]: addressBus) {
                std::cout << "  " << key << ": " << val << std::endl;
            }

            std::cout << "数据总线:" << std::endl;
            for (const auto &[key, val]: dataBus) {
                std::cout << "  " << key << ": " << val << std::endl;
            }

            std::cout << "控制总线:" << std::endl;
            for (const auto &[key, val]: controlBus) {
                std::cout << "  " << key << ": " << (val ? "true" : "false") << std::endl;
            }
        }
    };

    BIU biu; // 创建BIU实例

    // 执行单元（EU, Execution Unit）
    struct EU {
        _8086CPU* cpu; // 指向Simple8086实例的指针，以便访问寄存器和指令集

        EU(_8086CPU* _cpu) : cpu(_cpu) {}

        std::map<std::string, int16_t> registers; // 通用寄存器组

        // 解码并执行指令
        void decodeAndExecute() {
            int serialNumber = 0; // 当前执行指令的序号
            while (true) {
                if (!cpu->biu.instructionQueue.empty()) { // 指令队列非空则取出指令解码执行
                    std::unique_lock<std::mutex> lock(cpu->biu.queueMutex); // 锁住指令队列，保证互斥使用
                    std::string instruction = cpu->biu.instructionQueue.front().first; // 取出指令队列的一条指令
                    int memoryAddress = cpu->biu.instructionQueue.front().second; // 取出当前指令的内存地址
                    cpu->biu.instructionQueue.pop();
                    lock.unlock(); // 指令取出完成，解锁指令队列
                    std::cout << "-----------------------------------------" << std::endl;
                    std::cout << ++serialNumber << " : " << instruction << " （内存地址：" << memoryAddress << "）" << std::endl; // 输出当前执行指令
                    std::istringstream iss(instruction);

                    std::string op;
                    iss >> op; // 读取操作码
                    if (cpu->instructions.find(op) == cpu->instructions.end()) { // 没找到对应操作
                        std::cerr << "Invalid instruction: " << op << std::endl;
                        exit(1);
                    }

                    std::string arg1, arg2;
                    if (op != "HLT" && op != "JMP" && op != "PUSH" && op != "POP" && op != "DIV" && op != "MUL") {
                        // 对于非单操作数指令，读取第一个和第二个参数
                        iss >> arg1 >> arg2;
                    } else {
                        if (op == "JMP") {
                            iss >> arg2; // 读取跳转地址
                        } else if (op == "PUSH" || op == "POP") {
                            iss >> arg1; // 读取寄存器名
                        } else if (op == "DIV" || op == "MUL") {
                            iss >> arg2; // 读取除数或乘数
                        }
                    }

                    std::cout << "解码： ";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 模拟执行周期
                    execute(op, arg1, arg2); // 转到执行

                    cpu->printDebugState(); // 调试输出

                    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 保证BIU充足的取指时间，防止判断错误
                    if (cpu->biu.instructionQueue.empty()) { // 程序执行完成
                        std::cout << std::endl << "--- Program ends. ---" << std::endl;
                        break;
                    }
                }
            }
        }

        // 执行指令
        void execute(std::string op, std::string arg1, std::string arg2) {
            cpu->instructions[op](arg1, arg2);

            registers["FLAGS"] = (registers["FLAGS"] + 1) % 2; // 模拟标志状态寄存器的修改（每执行一次变一下）
        }

        // MOV指令实现（需判断是寄存器还是立即数）
        void mov(const std::string &reg, const std::string &s) {
            if (registers.find(reg) != registers.end()) { // 找到目的寄存器
                if (registers.find(s) != registers.end()) { // 找到源寄存器
                    cpu->biu.dataBus["data"] = registers[s]; // 寄存器数据上总线
                    registers[reg] = cpu->biu.dataBus["data"]; // 总线数据写入寄存器
                    std::cout << "操作码：" << "MOV " << "寄存器：" << reg << ", " << s << std::endl;
                    std::cout << "MOV " << registers[s] << " from " << s << " to " << reg << std::endl;
                } else { // 立即数
                    int val = std::stoi(s);
                    cpu->biu.dataBus["data"] = val; // 数据上总线
                    registers[reg] = cpu->biu.dataBus["data"]; // 总线数据写入寄存器
                    std::cout << "操作码：" << "MOV " << "寄存器：" << reg << " 值：" << val << std::endl;
                    std::cout << "MOV " << val << " to " << reg << std::endl;
                }
                cpu->biu.updateBuses("MOV", reg, s);
            } else { // 没找到目的寄存器
                std::cerr << "Invalid register for MOV: " << reg << std::endl;
                exit(1);
            }
        }

        // ADD指令实现（需判断是寄存器还是立即数）
        void add(const std::string &reg, const std::string &s) {
            if (registers.find(reg) != registers.end()) {
                if (registers.find(s) != registers.end()) {
                    cpu->biu.dataBus["data"] = registers[reg] + registers[s]; // ALU数据上总线
                    registers[reg] = cpu->biu.dataBus["data"]; // 总线数据写入寄存器
                    std::cout << "操作码：" << "ADD " << "寄存器：" << reg << ", " << s << std::endl;
                    std::cout << "ADD " << s << " to " << reg << ", result is " << registers[reg] << std::endl;
                } else {
                    int val = std::stoi(s);
                    cpu->biu.dataBus["data"] = registers[reg] + val; // ALU数据上总线
                    registers[reg] = cpu->biu.dataBus["data"]; // 总线数据写入寄存器
                    std::cout << "操作码：" << "ADD " << "寄存器：" << reg << " 值：" << val << std::endl;
                    std::cout << "ADD " << val << " to " << reg << ", result is " << registers[reg] << std::endl;
                }
                cpu->biu.updateBuses("ADD", reg, s);
            } else {
                std::cerr << "Invalid register for ADD: " << reg << std::endl;
                exit(1);
            }
        }

        // SUB指令实现
        void sub(const std::string &reg, const std::string &value) {
            int val = std::stoi(value);
            if (registers.find(reg) != registers.end()) {
                registers[reg] -= val;
                std::cout << "操作码：" << "SUB " << "寄存器：" << reg << " 值：" << val << std::endl;
                std::cout << "SUB " << val << " from " << reg << ", result is " << registers[reg] << std::endl;
                cpu->biu.updateBuses("SUB", reg, value);
            } else {
                std::cerr << "Invalid register for SUB: " << reg << std::endl;
                exit(1);
            }
        }

        // MUL指令实现（默认被乘数：AX）
        void mul(const std::string &value) {
            int val = std::stoi(value);
            int multiplicand = (registers.find("AX") != registers.end()) ? registers["AX"] : 0;
            registers["AX"] = multiplicand * val;
            std::cout << "操作码：" << "MUL " << "寄存器：" << "AX" << " 值：" << val << std::endl;
            std::cout << "MUL " << val << " to AX, result is " << registers["AX"] << std::endl;
            cpu->biu.updateBuses("MUL", "\\", value);
        }

        // DIV指令实现（默认被除数：AX）
        void div(const std::string &value) {
            int val = std::stoi(value);
            int dividend = (registers.find("AX") != registers.end()) ? registers["AX"] : 0; // 判0操作
            if (val == 0) {
                std::cerr << "Division by zero error" << std::endl;
                exit(1);
            }
            registers["AX"] = dividend / val;
            std::cout << "操作码：" << "DIV " << "寄存器：" << "AX" << " 值：" << val << std::endl;
            std::cout << "DIV " << val << " from AX, result is " << registers["AX"] << std::endl;
            cpu->biu.updateBuses("DIV", "\\", value);
        }

        // PUSH指令实现：将寄存器中的值压入栈中
        void push(const std::string &reg) {
            if (registers.find(reg) != registers.end()) {
                // 使用内存的一部分模拟栈，SP寄存器指向栈顶
                cpu->memory.push_back(std::to_string(registers[reg])); // 将寄存器值压入栈（这里简化为存入memory）
                registers["SP"]++; // 更新栈指针
                std::cout << "操作码：" << "PUSH " << "寄存器：" << reg << " 值：" << registers[reg] << std::endl;
                std::cout << "PUSH value " << registers[reg] << " from " << reg << " to stack, new SP is "
                          << registers["SP"] << std::endl;
            } else {
                std::cerr << "Invalid register for PUSH: " << reg << std::endl;
                exit(1);
            }
        }

        // POP指令实现：从栈中弹出一个值并存入寄存器中
        void pop(const std::string &reg) {
            if (registers.find(reg) != registers.end()) {
                if (registers["SP"] <= 0) {
                    std::cerr << "Stack underflow error" << std::endl;
                    exit(1);
                }
                // 从栈中弹出值（这里简化为从memory读取）
                registers["SP"]--; // 更新栈指针
                int value = std::stoi(cpu->memory.back()); // 获取栈顶值
                cpu->memory.pop_back(); // 移除栈顶元素
                registers[reg] = value; // 将值存入寄存器
                std::cout << "操作码：" << "POP " << "寄存器：" << reg << " 值：" << value << std::endl;
                std::cout << "POP value " << value << " to " << reg << ", new SP is " << registers["SP"] << std::endl;
            } else {
                std::cerr << "Invalid register for POP: " << reg << std::endl;
                exit(1);
            }
        }

        // JMP指令实现：跳转到指定地址继续执行指令
        void jmp(const std::string &addr) {
            int address = std::stoi(addr);
            if (address >= 0 && address < cpu->memory.size()) {
                std::cout << "操作码：" << "JMP " << "寄存器：" << "\\" << " 值：" << "\\" << std::endl;
                std::cout << "JMP to address " << address << std::endl;
            } else {
                std::cerr << "Invalid jump address: " << address << std::endl;
                exit(1);
            }
        }

        // HLT指令实现：停止执行后续指令
        void hlt() {
            std::cout << "操作码：" << "HLT " << "寄存器：" << "\\" << " 值：" << "\\" << std::endl;
            std::cout << "HLT executed, program halted." << std::endl;
            exit(0); // 退出程序，模拟系统挂起状态
        }
    };

    EU eu; // 创建EU实例

    // 调试输出
    void printDebugState() {
        std::cout << "寄存器:" << std::endl; // 寄存器状态
        for (const auto &[reg, val]: eu.registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }
        for (const auto &[reg, val]: biu.registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }
        for (const auto &[reg, val]: registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }

        std::cout << "指令队列:" << std::endl; // 指令队列内容
        std::queue<std::pair<std::string, int>> tempQueue = biu.instructionQueue;
        while (!tempQueue.empty()) {
            auto [instruction, memoryAddress] = tempQueue.front();
            tempQueue.pop();
            std::cout << "  " << instruction << std::endl;
        }

        std::cout << "内存:" << std::endl; // 内存内容
        for (int i = 0; i < memory.size(); ++i) {
            std::cout << "  [" << i << "]: " << memory[i] << std::endl;
        }
    }

public:
    _8086CPU() : biu(this), eu(this) {
        // 初始化寄存器、内存和指令集

        // 通用寄存器
        eu.registers["AX"] = 0; // 累加器
        eu.registers["BX"] = 0; // 基址寄存器
        eu.registers["CX"] = 0; // 计数器
        eu.registers["DX"] = 0; // 数据寄存器
        eu.registers["SP"] = 0; // 堆栈指针寄存器
        eu.registers["BP"] = 0; // 基址指针寄存器
        eu.registers["SI"] = 0; // 源变址寄存器
        eu.registers["DI"] = 0; // 目的变址寄存器

        eu.registers["FLAGS"] = 0; // 标志寄存器

        // 段寄存器
        biu.registers["CS"] = 0; // 代码段寄存器
        biu.registers["DS"] = 0; // 数据段寄存器
        biu.registers["SS"] = 0; // 堆栈段寄存器
        biu.registers["ES"] = 0; // 附加段寄存器

        biu.registers["IP"] = 0; // 指令指针寄存器
        registers["IR"] = 0; // 指令寄存器

        memory = {
                "MOV AX 15", // 将15加载到AX
                "MOV BX AX", // 将AX的值传给BX
                "ADD AX BX", // 将BX的值加给AX，此时AX为30
                "SUB AX 5", // 将AX的值减5，此时AX为25
                "JMP 6", // 跳过下一条指令
                "ADD DX 8", // 跳过
                "MUL 2", // 将AX的值乘以2，AX为50
                "DIV 10", // 将AX的值除以10，AX为5
                "PUSH BX", // 将BX的值入栈
                "POP AX", // 将栈中的值弹出到AX，AX为15
                "HLT" // 程序结束
        };

        instructions["MOV"] = [this](const std::string &reg, const std::string &value) { eu.mov(reg, value); };
        instructions["ADD"] = [this](const std::string &reg, const std::string &value) { eu.add(reg, value); };
        instructions["SUB"] = [this](const std::string &reg, const std::string &value) { eu.sub(reg, value); };
        instructions["MUL"] = [this](const std::string & /* ignored */, const std::string &value) { eu.mul(value); };
        instructions["DIV"] = [this](const std::string & /* ignored */, const std::string &value) { eu.div(value); };
        instructions["PUSH"] = [this](const std::string &reg, const std::string & /* ignored */) { eu.push(reg); };
        instructions["POP"] = [this](const std::string &reg, const std::string & /* ignored */) { eu.pop(reg); };
        instructions["JMP"] = [this](const std::string & /* ignored */, const std::string &addr) { eu.jmp(addr); };
        instructions["HLT"] = [this](const std::string & /* ignored */, const std::string & /* ignored */) { eu.hlt(); };
    }

    void run() {
        std::cout << "开始执行..." << std::endl;
        // 启动BIU和EU的线程
        std::thread biuThread = std::thread(&BIU::fetch, &biu);
        std::thread euThread = std::thread(&EU::decodeAndExecute, &eu);

        // 等待线程结束
        biuThread.join();
        euThread.join();
    }
};

int main() {
    _8086CPU cpu;
    cpu.run(); // 开始执行指令序列
    return 0;
}