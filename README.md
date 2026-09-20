<div align="center">
  <a href="https://wellkilo.github.io/Portfolio/">
    <img src="./assets/profile-hero.svg" width="100%" alt="He Dong — AI systems and agent infrastructure engineer" />
  </a>

  <br />

  <p>
    <a href="https://wellkilo.github.io/Portfolio/"><strong>Portfolio</strong></a>
    &nbsp;·&nbsp;
    <a href="https://wellkilo.github.io/"><strong>Notes</strong></a>
    &nbsp;·&nbsp;
    <a href="mailto:wellkilo@foxmail.com"><strong>Email</strong></a>
  </p>

  <p>
    <img src="https://img.shields.io/badge/Focus-Agent%20Infrastructure-0b172a?style=flat-square&labelColor=0b172a&color=2563eb" alt="Focus: Agent Infrastructure" />
    <img src="https://img.shields.io/badge/Research-Deep%20Reinforcement%20Learning-0b172a?style=flat-square&labelColor=0b172a&color=7c3aed" alt="Research: Deep Reinforcement Learning" />
    <a href="https://github.com/affaan-m/ECC/graphs/contributors"><img src="https://img.shields.io/badge/ECC-External%20Core%20Contributor-0b172a?style=flat-square&labelColor=0b172a&color=16a34a" alt="ECC: External Core Contributor" /></a>
    <img src="https://komarev.com/ghpvc/?username=wellkilo&style=flat-square&color=0891b2&label=PROFILE+VIEWS" alt="Profile views" />
  </p>
</div>

## About

I am **He Dong**, an M.S. student at the **Beijing Institute of Technology**, a former **Agent Evaluation Infrastructure intern at ByteDance**, and an **external core contributor to Everything Claude Code**. I build AI systems that can do useful engineering work while remaining observable, reviewable, and safe to operate.

My work sits at the intersection of **LLM agents**, **AI for code**, **developer tooling**, and **distributed reinforcement learning**. At ByteDance, I worked on AI-assisted engineering workflows that reduced an MR defect-tracing and code-review cycle from **30 minutes to 3 minutes**.

```text
observe  →  reproduce  →  locate  →  patch  →  verify
   evidence at every boundary · humans retain the final decision
```

## Open-source impact

> **External core contributor · [Everything Claude Code (ECC)](https://github.com/affaan-m/ECC)**<br />

<p>
  <a href="https://github.com/affaan-m/ECC/pulls?q=is%3Apr+is%3Amerged+author%3Awellkilo"><img src="https://img.shields.io/badge/Merged%20PRs-6-16a34a?style=flat-square&logo=github&logoColor=ffffff" alt="6 merged ECC pull requests" /></a>
  <a href="https://github.com/affaan-m/ECC/commits/main/?author=wellkilo"><img src="https://img.shields.io/badge/Upstream%20Commits-17-16a34a?style=flat-square&logo=git&logoColor=ffffff" alt="17 upstream ECC commits" /></a>
  <a href="https://github.com/affaan-m/ECC/graphs/contributors"><img src="https://img.shields.io/badge/Contributor%20Rank-Top%2010-16a34a?style=flat-square&logo=github&logoColor=ffffff" alt="Current top-10 ECC contributor by commit count" /></a>
  <a href="https://github.com/affaan-m/ECC"><img src="https://img.shields.io/github/stars/affaan-m/ECC?style=flat-square&logo=github&label=ECC%20stars&color=16a34a" alt="ECC GitHub stars" /></a>
</p>

I contribute production changes across ECC's metrics pipeline, cross-process dashboard, hook runtime, installer lifecycle, and continuous-learning system. The work is review-heavy and regression-driven: bounded resource use, atomic state transitions, explicit failure semantics, cross-platform behavior, and focused test evidence are treated as part of the feature rather than follow-up cleanup.

| Engineering lane | Representative upstream work | Verified impact |
| :--- | :--- | :--- |
| **Performance & observability** | [#3102](https://github.com/affaan-m/ECC/pull/3102) · session-cost snapshots<br />[#3064](https://github.com/affaan-m/ECC/pull/3064) · ECC2 output cursor | Replaced repeated full-history reads with **O(1) stable snapshots / O(delta) catch-up**, measuring an approximately **3,099×** steady-state speedup on a 40.5 MB fixture. Added a monotonic SQLite cursor for cross-process dashboard output. |
| **Hook & install lifecycle** | [#2992](https://github.com/affaan-m/ECC/pull/2992) · Claude hook ownership<br />[#2987](https://github.com/affaan-m/ECC/pull/2987) · silent hook contracts | Made manual hook installation ownership-aware and idempotent across install, upgrade, doctor, repair, and uninstall, backed by **177 focused tests**. Preserved explicit hook output while eliminating accidental payload echo and bounding input across lifecycle entrypoints. |
| **Failure containment & CI** | [#2971](https://github.com/affaan-m/ECC/pull/2971) · loss-safe archival<br />[#2194](https://github.com/affaan-m/ECC/pull/2194) · Unicode-safe CI | Prevented semantic failures from authorizing observation archival, retained data across timeout/interruption paths, and unblocked the repository's Unicode-safety gate without losing localized rating information. |

<a href="https://github.com/affaan-m/ECC/pulls?q=is%3Apr+is%3Amerged+author%3Awellkilo"><strong>Merged PRs</strong></a> ·
<a href="https://github.com/affaan-m/ECC/commits/main/?author=wellkilo"><strong>Upstream commits</strong></a> ·
<a href="https://github.com/affaan-m/ECC/graphs/contributors"><strong>Contributor graph</strong></a>

## Selected work

<table>
  <tr>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/RepoPilot">RepoPilot</a></h3>
      <img src="https://img.shields.io/badge/TypeScript-2563eb?style=flat-square&logo=typescript&logoColor=ffffff" alt="TypeScript" />
      <img src="https://img.shields.io/badge/AgentTeams-7c3aed?style=flat-square" alt="AgentTeams" />
      <img src="https://img.shields.io/badge/MCP-7c3aed?style=flat-square" alt="MCP" />
      <img src="https://img.shields.io/badge/PostgreSQL-2563eb?style=flat-square&logo=postgresql&logoColor=ffffff" alt="PostgreSQL" />
      <br /><br />
      An evidence-first AgentTeam for repository maintenance. It moves an issue or failed CI run toward a verified pull request while preserving decisions, approvals, tool calls, and rollback points in a tamper-evident execution chain.
      <br /><br />
      <a href="https://wellkilo.github.io/RepoPilot/"><strong>Interactive demo ↗</strong></a> ·
      <a href="https://github.com/wellkilo/repopilot-testbed/pull/4">Verified PR</a>
    </td>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/codemod-pilot">codemod-pilot</a></h3>
      <img src="https://img.shields.io/badge/Rust-0891b2?style=flat-square&logo=rust&logoColor=ffffff" alt="Rust" />
      <img src="https://img.shields.io/badge/tree--sitter-0891b2?style=flat-square" alt="tree-sitter" />
      <img src="https://img.shields.io/badge/AST-0891b2?style=flat-square" alt="AST" />
      <img src="https://img.shields.io/badge/CLI-0891b2?style=flat-square&logo=gnubash&logoColor=ffffff" alt="CLI" />
      <br /><br />
      A safe, example-driven codemod engine. Give it before-and-after snippets and it infers a structural transformation, scans a repository, previews the diff, and generates a rollback path before writing.
      <br /><br />
      <a href="https://github.com/wellkilo/codemod-pilot#-quick-start"><strong>Quick start ↗</strong></a>
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/DRL_MuJoCo">DRL MuJoCo</a></h3>
      <img src="https://img.shields.io/badge/Python-d97706?style=flat-square&logo=python&logoColor=ffffff" alt="Python" />
      <img src="https://img.shields.io/badge/PyTorch-d97706?style=flat-square&logo=pytorch&logoColor=ffffff" alt="PyTorch" />
      <img src="https://img.shields.io/badge/Ray-d97706?style=flat-square&logo=ray&logoColor=ffffff" alt="Ray" />
      <img src="https://img.shields.io/badge/Rust-d97706?style=flat-square&logo=rust&logoColor=ffffff" alt="Rust" />
      <br /><br />
      A distributed Actor–Learner training system for MuJoCo with parallel rollout collection, PPO optimization, multi-GPU experiments, a Rust replay buffer, and real-time experiment observability.
      <br /><br />
      <a href="https://github.com/wellkilo/DRL_MuJoCo"><strong>See the system ↗</strong></a>
    </td>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/conda-helper">conda-helper</a></h3>
      <img src="https://img.shields.io/badge/Python-2563eb?style=flat-square&logo=python&logoColor=ffffff" alt="Python" />
      <img src="https://img.shields.io/badge/Click-2563eb?style=flat-square" alt="Click" />
      <img src="https://img.shields.io/badge/Cross--platform-2563eb?style=flat-square" alt="Cross-platform" />
      <br /><br />
      A local-first CLI that makes Conda environment backup, restore, clone, offline packaging, cleanup, and diagnosis safer and more repeatable across Windows, macOS, and Linux.
      <br /><br />
      <a href="https://wellkilo.github.io/conda-helper/"><strong>Project site ↗</strong></a> ·
      <a href="https://pypi.org/project/conda-helper/">PyPI</a>
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/SlideCraft">SlideCraft</a></h3>
      <img src="https://img.shields.io/badge/Python-7c3aed?style=flat-square&logo=python&logoColor=ffffff" alt="Python" />
      <img src="https://img.shields.io/badge/HTML5-7c3aed?style=flat-square&logo=html5&logoColor=ffffff" alt="HTML5" />
      <img src="https://img.shields.io/badge/PPTX-7c3aed?style=flat-square&logo=microsoftpowerpoint&logoColor=ffffff" alt="PPTX" />
      <img src="https://img.shields.io/badge/AI%20Skill-7c3aed?style=flat-square" alt="AI Skill" />
      <br /><br />
      An AI presentation skill that produces animated, zero-dependency HTML decks and editable PowerPoint files through a deliberate theme system designed to avoid generic AI aesthetics.
    </td>
    <td width="50%" valign="top">
      <h3><a href="https://github.com/wellkilo/CaseAI">CaseAI</a></h3>
      <img src="https://img.shields.io/badge/TypeScript-7c3aed?style=flat-square&logo=typescript&logoColor=ffffff" alt="TypeScript" />
      <img src="https://img.shields.io/badge/Agent%20System-7c3aed?style=flat-square" alt="Agent System" />
      <img src="https://img.shields.io/badge/Testing-7c3aed?style=flat-square" alt="Testing" />
      <br /><br />
      An agent-based system that turns product requirement documents into structured test-case suites, connecting product intent with repeatable quality workflows.
    </td>
  </tr>
</table>

## Engineering principles

| Principle | What it means in practice |
| :--- | :--- |
| **Evidence before confidence** | Reproduce the failure, capture the trace, and make every conclusion inspectable. |
| **Safe autonomy** | Give agents useful tools, explicit boundaries, human approval gates, and reversible actions. |
| **Systems over demos** | Build typed contracts, tests, observability, and deployment paths around the model. |
| **Performance with a baseline** | Measure against a frozen reference and promote changes only when regressions are understood. |

## Technical toolkit

<p><strong>Languages</strong><br />
  <img src="https://img.shields.io/badge/Python-2563eb?style=flat-square&logo=python&logoColor=ffffff" alt="Python" />
  <img src="https://img.shields.io/badge/TypeScript-2563eb?style=flat-square&logo=typescript&logoColor=ffffff" alt="TypeScript" />
  <img src="https://img.shields.io/badge/Node.js-2563eb?style=flat-square&logo=nodedotjs&logoColor=ffffff" alt="Node.js" />
  <img src="https://img.shields.io/badge/Rust-2563eb?style=flat-square&logo=rust&logoColor=ffffff" alt="Rust" />
  <img src="https://img.shields.io/badge/Go-2563eb?style=flat-square&logo=go&logoColor=ffffff" alt="Go" />
  <img src="https://img.shields.io/badge/SQL-2563eb?style=flat-square&logo=postgresql&logoColor=ffffff" alt="SQL" />
  <img src="https://img.shields.io/badge/Shell-2563eb?style=flat-square&logo=gnubash&logoColor=ffffff" alt="Shell" />
</p>

<p><strong>Agent systems &amp; evaluation</strong><br />
  <img src="https://img.shields.io/badge/MCP-7c3aed?style=flat-square" alt="MCP" />
  <img src="https://img.shields.io/badge/AgentTeams-7c3aed?style=flat-square" alt="AgentTeams" />
  <img src="https://img.shields.io/badge/Tool%20Orchestration-7c3aed?style=flat-square" alt="Tool Orchestration" />
  <img src="https://img.shields.io/badge/Evaluation%20Harnesses-7c3aed?style=flat-square" alt="Evaluation Harnesses" />
  <img src="https://img.shields.io/badge/Auditable%20Execution-7c3aed?style=flat-square" alt="Auditable Execution" />
</p>

<p><strong>Code intelligence &amp; developer tooling</strong><br />
  <img src="https://img.shields.io/badge/tree--sitter-0891b2?style=flat-square" alt="tree-sitter" />
  <img src="https://img.shields.io/badge/AST%20Transformations-0891b2?style=flat-square" alt="AST Transformations" />
  <img src="https://img.shields.io/badge/CLI%20Design-0891b2?style=flat-square&logo=gnubash&logoColor=ffffff" alt="CLI Design" />
  <img src="https://img.shields.io/badge/GitHub%20Automation-0891b2?style=flat-square&logo=github&logoColor=ffffff" alt="GitHub Automation" />
  <img src="https://img.shields.io/badge/Cross--platform%20Tooling-0891b2?style=flat-square" alt="Cross-platform Tooling" />
</p>

<p><strong>ML &amp; distributed training</strong><br />
  <img src="https://img.shields.io/badge/PyTorch-d97706?style=flat-square&logo=pytorch&logoColor=ffffff" alt="PyTorch" />
  <img src="https://img.shields.io/badge/Ray-d97706?style=flat-square&logo=ray&logoColor=ffffff" alt="Ray" />
  <img src="https://img.shields.io/badge/PPO-d97706?style=flat-square" alt="PPO" />
  <img src="https://img.shields.io/badge/MuJoCo-d97706?style=flat-square" alt="MuJoCo" />
  <img src="https://img.shields.io/badge/Parallel%20Rollouts-d97706?style=flat-square" alt="Parallel Rollouts" />
  <img src="https://img.shields.io/badge/Multi--GPU-d97706?style=flat-square" alt="Multi-GPU" />
</p>

<p><strong>Backend &amp; data</strong><br />
  <img src="https://img.shields.io/badge/Fastify-2563eb?style=flat-square&logo=fastify&logoColor=ffffff" alt="Fastify" />
  <img src="https://img.shields.io/badge/FastAPI-2563eb?style=flat-square&logo=fastapi&logoColor=ffffff" alt="FastAPI" />
  <img src="https://img.shields.io/badge/PostgreSQL-2563eb?style=flat-square&logo=postgresql&logoColor=ffffff" alt="PostgreSQL" />
  <img src="https://img.shields.io/badge/SQLite-2563eb?style=flat-square&logo=sqlite&logoColor=ffffff" alt="SQLite" />
  <img src="https://img.shields.io/badge/WebSockets-2563eb?style=flat-square" alt="WebSockets" />
  <img src="https://img.shields.io/badge/Typed%20API%20Contracts-2563eb?style=flat-square" alt="Typed API Contracts" />
</p>

<p><strong>Reliability &amp; delivery</strong><br />
  <img src="https://img.shields.io/badge/OpenTelemetry-16a34a?style=flat-square&logo=opentelemetry&logoColor=ffffff" alt="OpenTelemetry" />
  <img src="https://img.shields.io/badge/Docker-16a34a?style=flat-square&logo=docker&logoColor=ffffff" alt="Docker" />
  <img src="https://img.shields.io/badge/GitHub%20Actions-16a34a?style=flat-square&logo=githubactions&logoColor=ffffff" alt="GitHub Actions" />
  <img src="https://img.shields.io/badge/Vitest-16a34a?style=flat-square&logo=vitest&logoColor=ffffff" alt="Vitest" />
  <img src="https://img.shields.io/badge/pytest-16a34a?style=flat-square&logo=pytest&logoColor=ffffff" alt="pytest" />
  <img src="https://img.shields.io/badge/Cross--platform%20CI-16a34a?style=flat-square" alt="Cross-platform CI" />
</p>

I am currently exploring reliable agent infrastructure, repository-scale code transformation, evaluation systems, and scalable reinforcement-learning workflows. I am always interested in thoughtful collaborations around these areas.
