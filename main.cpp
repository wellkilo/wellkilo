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
    std::vector<std::string> memory; // �洢ָ������ݵ��ڴ�ģ��
    std::map<std::string, int16_t> registers; // �Ĵ�����
    std::map<std::string, std::function<void(const std::string &, const std::string &)>> instructions; // ָ������Ӧ�Ĵ�����

    // ���߽ӿڵ�Ԫ��BIU, Bus Interface Unit��
    struct BIU {
        _8086CPU* cpu; // ָ��Simple8086ʵ����ָ�룬�Ա���ʼĴ������ڴ�

        BIU(_8086CPU* _cpu) : cpu(_cpu) {}

        std::queue<std::pair<std::string, int>> instructionQueue; // ָ�����
        std::mutex queueMutex; // �����������ڱ���ָ�����

        std::map<std::string, int16_t> registers; // �μĴ�����ָ��ָ�룬���ڲ��Ĵ���

        std::map<std::string, std::string> addressBus; // ��ַ����
        std::map<std::string, int16_t> dataBus; // ��������
        std::map<std::string, bool> controlBus; // ��������

        // ���ڴ���Ԥȡָ�ָ�����
        void fetch() {
            int instructionNumber = cpu->memory.size(); // ָ������
            while (true) {
                if (registers["IP"] >= instructionNumber) break; // �ڴ��е�ָ��ȫ��ȡ��

                if (instructionQueue.empty()) { // ָ�����Ϊ�գ�Ԥȡ��һ��ָ��
                    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // ģ��ȡָ����
                    std::unique_lock<std::mutex> lock(queueMutex); // ��סָ����У���֤����ʹ��
                    std::string instruction = cpu->memory[registers["IP"]];
                    instructionQueue.emplace(instruction, registers["IP"]); // ��ָ��Ԥȡ����ָ�����
                    lock.unlock(); // Ԥȡָ����ɣ�����ָ�����
                    // ��ȡ������Ͳ�����
                    std::istringstream iss(instruction);
                    std::string op;
                    iss >> op;
                    std::string opr;
                    iss >> opr;
                    if (op == "JMP") registers["IP"] = std::stoi(opr); // JMP����ת��ַ
                    else registers["IP"]++; // ָ��ָ���1
                }
            }
        }

        // ��������״̬
        void updateBuses(const std::string &operation, const std::string &registerName, const std::string &s) {
            // �������״̬
            addressBus.clear();
            dataBus.clear();
            controlBus.clear();

            // ���ݲ�����������
            if (operation == "MOV") { // ���ж��ǼĴ�������������
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) addressBus["source"] = s; // �ҵ��ǼĴ��� �����ַ����
                else addressBus["source"] = "\\"; // û�ҵ��Ĵ��� ��������
                addressBus["destination"] = registerName;
                if (cpu->eu.registers.find(s) != cpu->eu.registers.end()) dataBus["data"] = cpu->eu.registers[s]; // �ҵ��ǼĴ��� ������������
                else dataBus["data"] = std::stoi(s); // �ַ���תint����
                controlBus["read"] = true;
                controlBus["write"] = true;
            } else if (operation == "ADD") { // ���ж��ǼĴ�������������
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

            // �����������״̬
            printBusState();
        }

        // ��ӡ����״̬
        void printBusState() {
            std::cout << "��ַ����:" << std::endl;
            for (const auto &[key, val]: addressBus) {
                std::cout << "  " << key << ": " << val << std::endl;
            }

            std::cout << "��������:" << std::endl;
            for (const auto &[key, val]: dataBus) {
                std::cout << "  " << key << ": " << val << std::endl;
            }

            std::cout << "��������:" << std::endl;
            for (const auto &[key, val]: controlBus) {
                std::cout << "  " << key << ": " << (val ? "true" : "false") << std::endl;
            }
        }
    };

    BIU biu; // ����BIUʵ��

    // ִ�е�Ԫ��EU, Execution Unit��
    struct EU {
        _8086CPU* cpu; // ָ��Simple8086ʵ����ָ�룬�Ա���ʼĴ�����ָ�

        EU(_8086CPU* _cpu) : cpu(_cpu) {}

        std::map<std::string, int16_t> registers; // ͨ�üĴ�����

        // ���벢ִ��ָ��
        void decodeAndExecute() {
            int serialNumber = 0; // ��ǰִ��ָ������
            while (true) {
                if (!cpu->biu.instructionQueue.empty()) { // ָ����зǿ���ȡ��ָ�����ִ��
                    std::unique_lock<std::mutex> lock(cpu->biu.queueMutex); // ��סָ����У���֤����ʹ��
                    std::string instruction = cpu->biu.instructionQueue.front().first; // ȡ��ָ����е�һ��ָ��
                    int memoryAddress = cpu->biu.instructionQueue.front().second; // ȡ����ǰָ����ڴ��ַ
                    cpu->biu.instructionQueue.pop();
                    lock.unlock(); // ָ��ȡ����ɣ�����ָ�����
                    std::cout << "-----------------------------------------" << std::endl;
                    std::cout << ++serialNumber << " : " << instruction << " ���ڴ��ַ��" << memoryAddress << "��" << std::endl; // �����ǰִ��ָ��
                    std::istringstream iss(instruction);

                    std::string op;
                    iss >> op; // ��ȡ������
                    if (cpu->instructions.find(op) == cpu->instructions.end()) { // û�ҵ���Ӧ����
                        std::cerr << "Invalid instruction: " << op << std::endl;
                        exit(1);
                    }

                    std::string arg1, arg2;
                    if (op != "HLT" && op != "JMP" && op != "PUSH" && op != "POP" && op != "DIV" && op != "MUL") {
                        // ���ڷǵ�������ָ���ȡ��һ���͵ڶ�������
                        iss >> arg1 >> arg2;
                    } else {
                        if (op == "JMP") {
                            iss >> arg2; // ��ȡ��ת��ַ
                        } else if (op == "PUSH" || op == "POP") {
                            iss >> arg1; // ��ȡ�Ĵ�����
                        } else if (op == "DIV" || op == "MUL") {
                            iss >> arg2; // ��ȡ���������
                        }
                    }

                    std::cout << "���룺 ";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // ģ��ִ������
                    execute(op, arg1, arg2); // ת��ִ��

                    cpu->printDebugState(); // �������

                    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // ��֤BIU�����ȡָʱ�䣬��ֹ�жϴ���
                    if (cpu->biu.instructionQueue.empty()) { // ����ִ�����
                        std::cout << std::endl << "--- Program ends. ---" << std::endl;
                        break;
                    }
                }
            }
        }

        // ִ��ָ��
        void execute(std::string op, std::string arg1, std::string arg2) {
            cpu->instructions[op](arg1, arg2);

            registers["FLAGS"] = (registers["FLAGS"] + 1) % 2; // ģ���־״̬�Ĵ������޸ģ�ÿִ��һ�α�һ�£�
        }

        // MOVָ��ʵ�֣����ж��ǼĴ���������������
        void mov(const std::string &reg, const std::string &s) {
            if (registers.find(reg) != registers.end()) { // �ҵ�Ŀ�ļĴ���
                if (registers.find(s) != registers.end()) { // �ҵ�Դ�Ĵ���
                    cpu->biu.dataBus["data"] = registers[s]; // �Ĵ�������������
                    registers[reg] = cpu->biu.dataBus["data"]; // ��������д��Ĵ���
                    std::cout << "�����룺" << "MOV " << "�Ĵ�����" << reg << ", " << s << std::endl;
                    std::cout << "MOV " << registers[s] << " from " << s << " to " << reg << std::endl;
                } else { // ������
                    int val = std::stoi(s);
                    cpu->biu.dataBus["data"] = val; // ����������
                    registers[reg] = cpu->biu.dataBus["data"]; // ��������д��Ĵ���
                    std::cout << "�����룺" << "MOV " << "�Ĵ�����" << reg << " ֵ��" << val << std::endl;
                    std::cout << "MOV " << val << " to " << reg << std::endl;
                }
                cpu->biu.updateBuses("MOV", reg, s);
            } else { // û�ҵ�Ŀ�ļĴ���
                std::cerr << "Invalid register for MOV: " << reg << std::endl;
                exit(1);
            }
        }

        // ADDָ��ʵ�֣����ж��ǼĴ���������������
        void add(const std::string &reg, const std::string &s) {
            if (registers.find(reg) != registers.end()) {
                if (registers.find(s) != registers.end()) {
                    cpu->biu.dataBus["data"] = registers[reg] + registers[s]; // ALU����������
                    registers[reg] = cpu->biu.dataBus["data"]; // ��������д��Ĵ���
                    std::cout << "�����룺" << "ADD " << "�Ĵ�����" << reg << ", " << s << std::endl;
                    std::cout << "ADD " << s << " to " << reg << ", result is " << registers[reg] << std::endl;
                } else {
                    int val = std::stoi(s);
                    cpu->biu.dataBus["data"] = registers[reg] + val; // ALU����������
                    registers[reg] = cpu->biu.dataBus["data"]; // ��������д��Ĵ���
                    std::cout << "�����룺" << "ADD " << "�Ĵ�����" << reg << " ֵ��" << val << std::endl;
                    std::cout << "ADD " << val << " to " << reg << ", result is " << registers[reg] << std::endl;
                }
                cpu->biu.updateBuses("ADD", reg, s);
            } else {
                std::cerr << "Invalid register for ADD: " << reg << std::endl;
                exit(1);
            }
        }

        // SUBָ��ʵ��
        void sub(const std::string &reg, const std::string &value) {
            int val = std::stoi(value);
            if (registers.find(reg) != registers.end()) {
                registers[reg] -= val;
                std::cout << "�����룺" << "SUB " << "�Ĵ�����" << reg << " ֵ��" << val << std::endl;
                std::cout << "SUB " << val << " from " << reg << ", result is " << registers[reg] << std::endl;
                cpu->biu.updateBuses("SUB", reg, value);
            } else {
                std::cerr << "Invalid register for SUB: " << reg << std::endl;
                exit(1);
            }
        }

        // MULָ��ʵ�֣�Ĭ�ϱ�������AX��
        void mul(const std::string &value) {
            int val = std::stoi(value);
            int multiplicand = (registers.find("AX") != registers.end()) ? registers["AX"] : 0;
            registers["AX"] = multiplicand * val;
            std::cout << "�����룺" << "MUL " << "�Ĵ�����" << "AX" << " ֵ��" << val << std::endl;
            std::cout << "MUL " << val << " to AX, result is " << registers["AX"] << std::endl;
            cpu->biu.updateBuses("MUL", "\\", value);
        }

        // DIVָ��ʵ�֣�Ĭ�ϱ�������AX��
        void div(const std::string &value) {
            int val = std::stoi(value);
            int dividend = (registers.find("AX") != registers.end()) ? registers["AX"] : 0; // ��0����
            if (val == 0) {
                std::cerr << "Division by zero error" << std::endl;
                exit(1);
            }
            registers["AX"] = dividend / val;
            std::cout << "�����룺" << "DIV " << "�Ĵ�����" << "AX" << " ֵ��" << val << std::endl;
            std::cout << "DIV " << val << " from AX, result is " << registers["AX"] << std::endl;
            cpu->biu.updateBuses("DIV", "\\", value);
        }

        // PUSHָ��ʵ�֣����Ĵ����е�ֵѹ��ջ��
        void push(const std::string &reg) {
            if (registers.find(reg) != registers.end()) {
                // ʹ���ڴ��һ����ģ��ջ��SP�Ĵ���ָ��ջ��
                cpu->memory.push_back(std::to_string(registers[reg])); // ���Ĵ���ֵѹ��ջ�������Ϊ����memory��
                registers["SP"]++; // ����ջָ��
                std::cout << "�����룺" << "PUSH " << "�Ĵ�����" << reg << " ֵ��" << registers[reg] << std::endl;
                std::cout << "PUSH value " << registers[reg] << " from " << reg << " to stack, new SP is "
                          << registers["SP"] << std::endl;
            } else {
                std::cerr << "Invalid register for PUSH: " << reg << std::endl;
                exit(1);
            }
        }

        // POPָ��ʵ�֣���ջ�е���һ��ֵ������Ĵ�����
        void pop(const std::string &reg) {
            if (registers.find(reg) != registers.end()) {
                if (registers["SP"] <= 0) {
                    std::cerr << "Stack underflow error" << std::endl;
                    exit(1);
                }
                // ��ջ�е���ֵ�������Ϊ��memory��ȡ��
                registers["SP"]--; // ����ջָ��
                int value = std::stoi(cpu->memory.back()); // ��ȡջ��ֵ
                cpu->memory.pop_back(); // �Ƴ�ջ��Ԫ��
                registers[reg] = value; // ��ֵ����Ĵ���
                std::cout << "�����룺" << "POP " << "�Ĵ�����" << reg << " ֵ��" << value << std::endl;
                std::cout << "POP value " << value << " to " << reg << ", new SP is " << registers["SP"] << std::endl;
            } else {
                std::cerr << "Invalid register for POP: " << reg << std::endl;
                exit(1);
            }
        }

        // JMPָ��ʵ�֣���ת��ָ����ַ����ִ��ָ��
        void jmp(const std::string &addr) {
            int address = std::stoi(addr);
            if (address >= 0 && address < cpu->memory.size()) {
                std::cout << "�����룺" << "JMP " << "�Ĵ�����" << "\\" << " ֵ��" << "\\" << std::endl;
                std::cout << "JMP to address " << address << std::endl;
            } else {
                std::cerr << "Invalid jump address: " << address << std::endl;
                exit(1);
            }
        }

        // HLTָ��ʵ�֣�ִֹͣ�к���ָ��
        void hlt() {
            std::cout << "�����룺" << "HLT " << "�Ĵ�����" << "\\" << " ֵ��" << "\\" << std::endl;
            std::cout << "HLT executed, program halted." << std::endl;
            exit(0); // �˳�����ģ��ϵͳ����״̬
        }
    };

    EU eu; // ����EUʵ��

    // �������
    void printDebugState() {
        std::cout << "�Ĵ���:" << std::endl; // �Ĵ���״̬
        for (const auto &[reg, val]: eu.registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }
        for (const auto &[reg, val]: biu.registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }
        for (const auto &[reg, val]: registers) {
            std::cout << "  " << reg << ": " << val << std::endl;
        }

        std::cout << "ָ�����:" << std::endl; // ָ���������
        std::queue<std::pair<std::string, int>> tempQueue = biu.instructionQueue;
        while (!tempQueue.empty()) {
            auto [instruction, memoryAddress] = tempQueue.front();
            tempQueue.pop();
            std::cout << "  " << instruction << std::endl;
        }

        std::cout << "�ڴ�:" << std::endl; // �ڴ�����
        for (int i = 0; i < memory.size(); ++i) {
            std::cout << "  [" << i << "]: " << memory[i] << std::endl;
        }
    }

public:
    _8086CPU() : biu(this), eu(this) {
        // ��ʼ���Ĵ������ڴ��ָ�

        // ͨ�üĴ���
        eu.registers["AX"] = 0; // �ۼ���
        eu.registers["BX"] = 0; // ��ַ�Ĵ���
        eu.registers["CX"] = 0; // ������
        eu.registers["DX"] = 0; // ���ݼĴ���
        eu.registers["SP"] = 0; // ��ջָ��Ĵ���
        eu.registers["BP"] = 0; // ��ַָ��Ĵ���
        eu.registers["SI"] = 0; // Դ��ַ�Ĵ���
        eu.registers["DI"] = 0; // Ŀ�ı�ַ�Ĵ���

        eu.registers["FLAGS"] = 0; // ��־�Ĵ���

        // �μĴ���
        biu.registers["CS"] = 0; // ����μĴ���
        biu.registers["DS"] = 0; // ���ݶμĴ���
        biu.registers["SS"] = 0; // ��ջ�μĴ���
        biu.registers["ES"] = 0; // ���ӶμĴ���

        biu.registers["IP"] = 0; // ָ��ָ��Ĵ���
        registers["IR"] = 0; // ָ��Ĵ���

        memory = {
                "MOV AX 15", // ��15���ص�AX
                "MOV BX AX", // ��AX��ֵ����BX
                "ADD AX BX", // ��BX��ֵ�Ӹ�AX����ʱAXΪ30
                "SUB AX 5", // ��AX��ֵ��5����ʱAXΪ25
                "JMP 6", // ������һ��ָ��
                "ADD DX 8", // ����
                "MUL 2", // ��AX��ֵ����2��AXΪ50
                "DIV 10", // ��AX��ֵ����10��AXΪ5
                "PUSH BX", // ��BX��ֵ��ջ
                "POP AX", // ��ջ�е�ֵ������AX��AXΪ15
                "HLT" // �������
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
        std::cout << "��ʼִ��..." << std::endl;
        // ����BIU��EU���߳�
        std::thread biuThread = std::thread(&BIU::fetch, &biu);
        std::thread euThread = std::thread(&EU::decodeAndExecute, &eu);

        // �ȴ��߳̽���
        biuThread.join();
        euThread.join();
    }
};

int main() {
    _8086CPU cpu;
    cpu.run(); // ��ʼִ��ָ������
    return 0;
}