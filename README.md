<div align="center">
  <a href="https://wellkilo.github.io/">
    <img src="https://wellkilo.github.io/images/profile-hero.svg" width="100%" alt="He Dong — I build AI agents that stay observable, reviewable, and safe to operate. Live open-source metrics for Everything Claude Code, XERJ Corpus Hub selected contributor, and an MR review cycle cut from 30 to 3 minutes at ByteDance." />
  </a>

  <p>
    <a href="https://wellkilo.github.io/"><strong>Website</strong></a>
    &nbsp;·&nbsp;
    <a href="mailto:wellkilo@foxmail.com"><strong>Email</strong></a>
  </p>

  <p>
    <a href="https://github.com/affaan-m/ECC/graphs/contributors"><img src="https://img.shields.io/badge/ECC-External%20Core%20Contributor-16a34a?style=flat-square&labelColor=0b172a" alt="ECC: External Core Contributor" /></a>
    <a href="https://hub.xerj.org/contributors/wellkilo"><img src="https://img.shields.io/badge/XERJ%20Corpus%20Hub-Selected%20Contributor-7c3aed?style=flat-square&labelColor=0b172a" alt="XERJ Corpus Hub: Selected Contributor" /></a>
    <img src="https://img.shields.io/badge/ByteDance-2%C3%97%20AI%20Eng%20%26%20Eval-2563eb?style=flat-square&labelColor=0b172a" alt="ByteDance: two AI engineering and evaluation internships" />
    <img src="https://img.shields.io/badge/M.S.-Beijing%20Institute%20of%20Technology-0891b2?style=flat-square&labelColor=0b172a" alt="M.S. at Beijing Institute of Technology" />
  </p>
</div>

**He Dong (董赫)** — M.S. student at BIT, previously two AI engineering & evaluation internships at ByteDance. I build agent infrastructure, evaluation systems, and code-intelligence tools, and I ship the tests, traces, and rollback paths as part of the feature.

## Open source

**[Everything Claude Code](https://github.com/affaan-m/ECC)** · external core contributor

<p>
  <a href="https://github.com/affaan-m/ECC/pulls?q=is%3Apr+is%3Amerged+author%3Awellkilo"><img src="https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Fwellkilo.github.io%2Fdata%2Fmetrics.json&query=%24.display.mergedPrs&label=merged%20PRs&style=flat-square&color=16a34a&labelColor=0b172a&logo=github&logoColor=ffffff&cacheSeconds=3600" alt="Merged ECC pull requests" /></a>
  <a href="https://github.com/affaan-m/ECC/commits/main/?author=wellkilo"><img src="https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Fwellkilo.github.io%2Fdata%2Fmetrics.json&query=%24.display.commits&label=upstream%20commits&style=flat-square&color=16a34a&labelColor=0b172a&logo=git&logoColor=ffffff&cacheSeconds=3600" alt="Upstream ECC commits" /></a>
  <a href="https://github.com/affaan-m/ECC/graphs/contributors"><img src="https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Fwellkilo.github.io%2Fdata%2Fmetrics.json&query=%24.display.rank&label=contributor%20rank&style=flat-square&color=16a34a&labelColor=0b172a&logo=github&logoColor=ffffff&cacheSeconds=3600" alt="ECC contributor rank by commits" /></a>
  <a href="https://github.com/affaan-m/ECC"><img src="https://img.shields.io/github/stars/affaan-m/ECC?style=flat-square&logo=github&logoColor=ffffff&label=stars&color=16a34a&labelColor=0b172a" alt="ECC GitHub stars" /></a>
</p>

| Area | What shipped | PRs |
| :--- | :--- | :--- |
| **Performance** | O(1) session-cost snapshots with O(Δ) catch-up: **~3,099×** faster steady state on a 40.5 MB fixture | [#3102](https://github.com/affaan-m/ECC/pull/3102) [#3064](https://github.com/affaan-m/ECC/pull/3064) |
| **Hook lifecycle** | Ownership-aware, idempotent hook install / upgrade / doctor / repair / uninstall, backed by **177** focused tests | [#2992](https://github.com/affaan-m/ECC/pull/2992) [#2987](https://github.com/affaan-m/ECC/pull/2987) |
| **Harness integrity** | Project Hookify rules enforced in Claude's real hook lifecycle; deterministic Cursor dispatch; Python consumer audits | [#3055](https://github.com/affaan-m/ECC/pull/3055) [#3001](https://github.com/affaan-m/ECC/pull/3001) [#3007](https://github.com/affaan-m/ECC/pull/3007) |
| **Eval & CI** | Capability gates before side effects, loss-safe archival, Unicode-safe CI | [#2954](https://github.com/affaan-m/ECC/pull/2954) [#2971](https://github.com/affaan-m/ECC/pull/2971) [#2194](https://github.com/affaan-m/ECC/pull/2194) |

**[XERJ Corpus Hub](https://hub.xerj.org/contributors/wellkilo)** · selected contributor — my ECC commit [`a50ee13`](https://github.com/affaan-m/ECC/commit/a50ee13caca4b291f4279550d06416b570d062d1), *execute bounded local Hookify rules in lifecycle hooks* (from [#3055](https://github.com/affaan-m/ECC/pull/3055) · 19 files · 2,822 lines), was selected for the open reference memory that AI coding agents search before they write code.

## Selected work

| Project | What it does | Stack |
| :--- | :--- | :--- |
| **[RepoPilot](https://github.com/wellkilo/RepoPilot)**<br /><sub>[demo ↗](https://wellkilo.github.io/RepoPilot/)</sub> | Takes an issue or a failed CI run to a [verified pull request](https://github.com/wellkilo/repopilot-testbed/pull/4), recording every decision, approval, and tool call in a tamper-evident chain | TypeScript · MCP · PostgreSQL |
| **[EmbodiGen](https://github.com/wellkilo/embodigen)**<br /><sub>[showcase ↗](https://wellkilo.github.io/embodigen/)</sub> | One task sentence in, photorealistic robot-learning episodes with exact action labels out; a validator rejects renders that drift from the ground-truth geometry | Python · zero deps |
| **[codemod-pilot](https://github.com/wellkilo/codemod-pilot)** | Infers AST rewrite rules from before/after examples, previews the diff, and keeps a rollback path before writing | Rust · tree-sitter |
| **[codex-jev-preflight](https://github.com/wellkilo/codex-jev-preflight)**<br /><sub>[demo ↗](https://wellkilo.github.io/codex-jev-preflight/)</sub> | Fail-open Codex hook that classifies every task by type, complexity, and risk before work starts | Python · Codex hooks |
| **[DRL MuJoCo](https://github.com/wellkilo/DRL_MuJoCo)** | Distributed Actor–Learner PPO on Ray with parallel rollouts, multi-GPU training, and a Rust replay buffer | PyTorch · Ray · Rust |
| **[conda-helper](https://github.com/wellkilo/conda-helper)**<br /><sub>[PyPI ↗](https://pypi.org/project/conda-helper/)</sub> | Safe, cross-platform Conda backup, restore, clone, offline packing, and diagnosis | Python · Click |

<sub>Also: [Quorum](https://github.com/wellkilo/Quorum) (low-interruption group agent, AWS Agents for Humans) · [SlideCraft](https://github.com/wellkilo/SlideCraft) (AI slide skill, HTML + PPTX) · [CaseAI](https://github.com/wellkilo/CaseAI) (PRD → test suites)</sub>

## Toolkit

<img src="https://skillicons.dev/icons?i=py,ts,rust,go,nodejs,pytorch,fastapi,react,postgres,sqlite,docker,githubactions,linux&perline=13" alt="Python, TypeScript, Rust, Go, Node.js, PyTorch, FastAPI, React, PostgreSQL, SQLite, Docker, GitHub Actions, Linux" />

<sub>Plus MCP · AgentTeams · evaluation harnesses · Ray · tree-sitter · OpenTelemetry · Fastify</sub>

---

<sub>Exploring reliable agent infrastructure, repository-scale code transformation, and scalable RL. Open to collaboration — <a href="mailto:wellkilo@foxmail.com">wellkilo@foxmail.com</a></sub>
<img src="https://hits.sh/github.com/wellkilo.svg?style=flat-square&label=views&color=0891b2&extraCount=1360" alt="Profile views" align="right" />
