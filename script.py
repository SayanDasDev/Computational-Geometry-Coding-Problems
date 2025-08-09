import os
import re

README_PATH = "README.md"
WEEK_FOLDER_PREFIX = "week"

TEMPLATE = """
## 📂 Week {week_num}: {title}

### 📜 Problem Statement
> {problem_statement}

---

### 📁 Folder
[🔗 Go to Week {week_num} folder](./{folder_name}/)

---

### 📄 Demo Input
<details>
<summary>Click to view sample input</summary>

```
{input_content}
```

</details>

---
"""

def append_week_to_readme(week_num, title):
    folder_name = f"{WEEK_FOLDER_PREFIX}{week_num}"
    problem_file = os.path.join(folder_name, "problem.txt")

    if not os.path.exists(problem_file):
        print(f"❌ Missing problem.txt in {folder_name}")
        return

    # Find any file starting with 'input' and ending with .txt (including variations like input1_something.txt)
    input_file = None
    pattern = re.compile(r"^input.*\.txt$", re.IGNORECASE)
    for file in os.listdir(folder_name):
        if pattern.match(file):
            input_file = os.path.join(folder_name, file)
            break

    if input_file is None:
        print(f"❌ No input*.txt file found in {folder_name}")
        return

    with open(problem_file, "r", encoding="utf-8") as f:
        problem_statement = f.read().strip()

    with open(input_file, "r", encoding="utf-8") as f:
        input_content = f.read().strip()

    new_section = TEMPLATE.format(
        week_num=week_num,
        title=title,
        problem_statement=problem_statement,
        folder_name=folder_name,
        input_content=input_content
    )

    if os.path.exists(README_PATH):
        with open(README_PATH, "a", encoding="utf-8") as f:
            f.write(new_section)
        print(f"✅ Added Week {week_num} to README.md")
    else:
        with open(README_PATH, "w", encoding="utf-8") as f:
            f.write(f"# Weekly Coding Problems 📚\n\n{new_section}")
        print(f"📄 Created README.md and added Week {week_num}")

def add_toc():
    if not os.path.exists(README_PATH):
        print("❌ README.md not found")
        return

    with open(README_PATH, "r", encoding="utf-8") as f:
        content = f.readlines()

    # Find week headings
    week_pattern = re.compile(r"^## 📂 Week (\d+): (.+)$")
    toc_entries = []
    for line in content:
        match = week_pattern.match(line.strip())
        if match:
            week_num, title = match.groups()
            anchor = f"#📂-week-{week_num}-{title.lower().replace(' ', '-') }"
            toc_entries.append(f"- [Week {week_num}: {title}]({anchor})\n")

    if not toc_entries:
        print("ℹ️ No week sections found in README.md")
        return

    toc_block = ["## 📑 Table of Contents\n\n"] + toc_entries + ["\n"]

    # Remove existing TOC if any
    if content and content[0].startswith("## 📑 Table of Contents"):
        while content and content[0].strip():
            content.pop(0)
        while content and content[0] == "\n":
            content.pop(0)

    new_content = toc_block + content

    with open(README_PATH, "w", encoding="utf-8") as f:
        f.writelines(new_content)
    print("✅ Table of Contents added to README.md")

if __name__ == "__main__":
    choice = input("Enter mode (add_week / toc): ").strip().lower()
    if choice == "add_week":
        week_num = input("Enter week number: ").strip()
        title = input("Enter problem title: ").strip()
        append_week_to_readme(week_num, title)
    elif choice == "toc":
        add_toc()
    else:
        print("❌ Unknown mode")
