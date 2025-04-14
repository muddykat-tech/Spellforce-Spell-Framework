# Spellforce Spell Framework Coding Style

This style is based on the Linux kernel coding style.

## 1) Indentation and Switch Statements

Indentations should be 4 spaces.

As we are a heretic movement when creating a Switch statement, they should follow a clear and consistent indentation style where subordinate `case` blocks are **indented** and wrapped in **braces** to clearly delineate their scope.
Each `case` should be followed by a block wrapped in `{}`. Fallthroughs are **only acceptable** if clearly grouped and scoped together using braces:
```c
switch (suffix)
{
    case 'G':
    case 'g':
    {
        mem <<= 30;
        break;
    }
    case 'M':
    case 'm':
    {
        mem <<= 20;
        break;
    }
    default:
    {
        break;
    }
}
```

Fallthrough without braces or implicit continuation between cases is not permitted, as it reduces clarity and increases the chance of bugs:

```c
switch (input)
{
    case 'a':
        do_a();
    case 'b':
        do_b();  // Not clear if fallthrough is intended
        break;
}
```

Don't put multiple statements on a single line unless you have something to hide:

```c
if (condition) do_this;
  do_something_everytime;
```

Don't put multiple assignments on a single line either.
Avoid tricky expressions.
Don't leave whitespace at the end of lines.

## 2) Breaking Long Lines and Strings

Coding style is all about readability and maintainability using commonly available tools.

The limit on the length of lines is **120 columns** and this is strongly preferred.

Statements longer than 120 columns will be broken into sensible chunks unless exceeding 120 columns significantly increases readability and does not hide information. Descendants are always shorter than the parent and placed substantially to the right.

Acceptable
```c
if (user_is_admin(user) &&
    user->permissions.can_edit &&
    user->permissions.can_delete &&
    !user->is_suspended)
    perform_admin_action(user);
```

Rejected
```c
if (user_is_admin(user) && user->permissions.can_edit && user->permissions.can_delete && !user->is_suspended)
    perform_admin_action(user);
```

## 3) Placing Braces and Spaces

The preferred brace placement follows the **Allman style**, where opening braces `{` are placed on a **new line** for all blocks.

This style puts the brace associated with a control statement on the next line, indented to the same level as the control statement. Statements within the braces are indented to the next level.

For example this is an example for the correct placement for braces.
```c
while (x == y)
{
    something();
    something_else();
}

final_thing();
```


## 4) Naming

Use short, meaningful names. C is a Spartan language:

- Use `tmp` not `ThisVariableIsATemporaryCounter`
- Global functions should be descriptive: `count_active_users()`, not `cntusr()`
- Don’t use Hungarian notation
- Use `i` or `tmp` for simple locals

Avoid growing functions that make variable naming hard.

### 5) Functions

Functions should be concise, focused, and do **one thing well**. Ideally, they should be no longer than 72 lines.

- **Shorter is better**, especially for functions with deep nesting or complex logic.
- If the function is conceptually simple (like a long but flat switch statement), longer length is acceptable.
- Use **helper functions** to break up complexity and improve readability. Give them clear, descriptive names.
- Avoid having **more than 5–10 local variables**; exceeding this suggests the function is doing too much.

**Exception**: When duplicating game logic from **Spellforce**, function size and complexity rules may be relaxed **to preserve correctness**. However, this style guide should still be followed **where possible** without affecting game behavior.

**Best practices:**
- Separate functions with a blank line.
- Function prototypes should include parameter names for clarity.

```c
int system_is_up(void)
{
    return system_state == SYSTEM_RUNNING;
}
EXPORT_SYMBOL(system_is_up);
```

### 6) Commenting

Comments should **explain what** the code does **not how** it works. If your code is hard to understand, improve the code itself rather than relying on comments to explain it.
However, when writing **example mods**, comments should be used to explain how the code works, as these serve as educational material for others.

- Avoid over-commenting and inline comments inside functions unless absolutely necessary.
- Prefer placing comments **above functions**, summarizing what they do and **why** (if not obvious).
- If you need to explain a specific tricky or non-obvious section, keep it brief and relevant.

#### Doxygen-Compatible Format

Comments should be written in a way that **Doxygen can parse** for documentation generation — especially for public APIs or important internal functions.

An example of a Doxygen Compatibale Comment.
```c
/**
 * function_name - Short description
 * @param1: Description of first parameter
 * @param2: Description of second parameter
 *
 * Longer description if needed.
 *
 * Return: What the function returns
 */
```

#### Comment Styles

Use this format for multi-line comments:

```c
/*
 * This is the preferred style for multi-line
 * comments in general Linux-style code.
 */
```

#### Data Commenting

- Always comment data structures and variables.
- Use **one declaration per line** to allow individual comments:

```c
int socket_fd;      /* File descriptor for the socket */
int retry_count;    /* Number of allowed retry attempts */
```

Avoid:

```c
int socket_fd, retry_count;  /* Bad: can't comment both easily */
```

Keep it concise, meaningful, and **documented in a format that tooling like Doxygen can understand**.

### 7) Macros and Enums

#### Enums

Enums should be named like **datatypes** with the first letter of each word capitalized, and **entries** within the enum should be **capitalized** (using uppercase and underscores to separate words). The name of the enum itself should reflect the type or category it represents.

- **Acceptable Enum Name:** `SpellFlagKey`
- **Unacceptable Enum Name:** `SPELLFLAGKEY`

Example:
```c
typedef enum
{
    CHECK_SPELLS_BEFORE_CHECK_BATTLE = 0x04,
    CHECK_SPELLS_BEFORE_JOB2 = 0x02,
    UNFREEZE = 0x1
} SpellFlagKey;
```

In this example, the enum `SpellFlagKey` is clearly named to reflect its purpose (as a datatype), and the entries are capitalized to distinguish them as constants.
To simplify usage, enums should be defined using `typedef` to avoid having to declare variables as `enum SpellFlagKey key`. Instead, the variable can be declared directly as `SpellFlagKey key`.

#### Macros

Macros should be **capitalized** entirely (using uppercase and underscores to separate words). Macros can be used for constants, labels, or simple inline code, but **avoid using math operations** within macros. This is because math operations inside macros can quickly get out of hand and make the macro's performance and effect hard to understand or debug.

Example of a good macro:

```c
#define MAX_SPELLS 100
#define PLAYER_SPEED 10
```

This is straightforward, with clear constant values.

Avoid complex macros involving math operations, as it obfuscates how expensive the macro is and can introduce hard-to-track bugs. For instance:

**Avoid:**

```c
#define SQUARE(x) (x * x)
```

Instead, consider using a **function** for calculations, as it is easier to understand, debug, and optimize:

```c
inline int square(int x) {
    return x * x;
}
```

### 8) Memory Allocation

Memory management is crucial in ensuring that the game runs efficiently and avoids unnecessary overhead.
When writing code, it's important to **avoid direct memory allocation** as much as possible because it can negatively affect the game's memory state, potentially leading to crashes or instability.

#### General Guidelines

1. **Avoid Direct Memory Allocation**
   Direct memory allocation (e.g., using `malloc`, `calloc`, or `realloc`) should be avoided unless absolutely necessary. Constantly allocating and deallocating memory at runtime can be inefficient and lead to fragmented memory, which can make the game more prone to errors or slowdowns.

2. **Prefer Stack Allocation**
   Whenever possible, use **stack memory** instead of heap memory. The stack is managed automatically, and objects are cleaned up when they go out of scope. This avoids the risk of memory leaks and unnecessary allocation overhead.

   Example:

   ```c
   void example_function() {
       int local_var = 5;  // Allocated on the stack
       // Do something with local_var
   }
   ```

   Stack memory is **fast** and is generally used for short-lived variables. It should be your preferred method for memory allocation whenever possible.

3. **Pre-Allocate Memory at Compile Time**
   If dynamic memory allocation is required, try to **pre-allocate memory at compile time** rather than allocating it during runtime. This ensures that memory is reserved ahead of time, minimizing performance hits from dynamic allocation.

   Example (using `static` allocation):

   ```c
   #define MAX_ITEMS 100       // Avoid Magic Numbers
   int item_array[MAX_ITEMS];  // Pre-allocated array
   ```

   In this example, the array is allocated at compile time, ensuring it is available when the game runs without the need for runtime allocation.
   If data must persist for the lifetime of the program, consider using **static** or **global variables**. These variables are automatically allocated at compile time and persist for the entire execution of the program.

6. **When Heap Allocation is Absolutely Necessary**
   If dynamic memory allocation (heap allocation) is **required** for complex data structures or objects that cannot be pre-allocated, it is crucial to manage the allocated memory carefully. Ensure that every allocation has a corresponding deallocation and that there are no memory leaks.

   Example of proper heap allocation:

   ```c
   void* ptr = malloc(sizeof(MyStruct));
   if (ptr == NULL) {
       // Handle memory allocation failure
   }

   // Use the allocated memory

   free(ptr);  // Free memory when done
   ```

   **Important:** Ensure this does not impact Spellforce and always check for allocation failures and ensure you free allocated memory when it is no longer needed.

#### Summary

- **Avoid direct memory allocation** as much as possible to ensure stable memory management and prevent fragmentation.
- **Prefer stack allocation** for short-lived variables and function calls.
- **Pre-allocate memory** at compile time whenever feasible to reduce runtime overhead.
- **Minimize heap allocations** and ensure proper memory management by freeing memory when it's no longer in use.

9) Inline assembly
-------------------
Inline assembly should **only be used** when absolutely necessary specifically when **writing hooks for SpellForce functions**. In all other cases, prefer writing your logic in C or C++.
If inline assembly is required for a hook:
- Keep it **minimal**
- Ensure it is **well-documented**
- Keep it **self-contained**
- Always **preserve the calling convention** and **register state** unless explicitly modifying them

This ensures that even low-level code can be maintained by other developers and prevents breaking the game engine’s expected behavior.

### Valid Example of Inline Assembly:

```c
static void __declspec(naked) menuload_hook_beta()
{
    asm("push %%edi         \n\t"  // Store EDI (typically points to CAppMenu instance)
        "call %P0           \n\t"  // Call our hook function (which can also call the original)
        "pop %%edi          \n\t"  // Restore EDI after hook logic is done
        "jmp *%1            \n\t"  // Jump back to the return address after the original hook point
        : : "i"(sf_menu_hook), "o"(g_menu_return_addr));
}
```

## Notes:

- This pattern ensures the original function's flow is resumed properly after the hook.
- Make sure to understand the function prologue/epilogue you’re hooking into, so you don’t inadvertently corrupt the stack or registers.
- Always test hooks thoroughly, as inline assembly bugs are often subtle and hard to diagnose.

## When NOT to Use Inline Assembly:
- Do not use inline assembly for micro-optimizations. Modern compilers are often better at optimizing plain C/C++.

### Alternative to Inline Assembly: Full Function Rewrite

Before opting to use inline assembly, **always evaluate whether the original function's behavior can be replicated in pure C or C++**.
In many cases, you can avoid the complexity and risks of inline assembly by simply rewriting the function and redirecting execution to your version using a jump or call hook.

This method improves code maintainability but also avoids undefined behavior caused by low-level manipulation of registers or the stack.

### Example: Redirecting to a C++ Replacement

Instead of writing inline assembly to hook and manipulate execution flow, you can safely redirect the function to your C++ implementation:

```c++
static void initialize_spellrefresh_hook()
{
    // State how much memory to overwrite, in this case it is the first 9 bytes of the target function
    ASI::MemoryRegion refresh_spell_mreg(ASI::AddrOf(0x329f90), 9);
    // Remove Memory Write Protection
    ASI::BeginRewrite(refresh_spell_mreg);

    // Overwrite the original function with NOPs and a JMP to our replacement
    *(unsigned char *)(ASI::AddrOf(0x329f90)) = 0x90; // NOP
    *(unsigned char *)(ASI::AddrOf(0x329f91)) = 0x90; // NOP
    *(unsigned char *)(ASI::AddrOf(0x329f92)) = 0x90; // NOP
    *(unsigned char *)(ASI::AddrOf(0x329f93)) = 0x90; // NOP
    *(unsigned char *)(ASI::AddrOf(0x329f94)) = 0xE9; // JMP opcode
    *(int *)(ASI::AddrOf(0x329f95)) = (int)(&sf_refresh_hook) - ASI::AddrOf(0x329f99); // Offset to Jump.

    // Restore Memory Write Protection
    ASI::EndRewrite(refresh_spell_mreg);
}
```

### Why This Is Preferred:

- `sf_refresh_hook` can now be a **clean, pure C or C++ function** that replicates or extends the original behavior.
- You're avoiding **inline assembly entirely**, unless strictly required.
- The redirection is transparent to the rest of the system.

Only fall back to inline assembly if replicating the original logic is impractical or impossible due to complexity or required access to specific registers at specific times.
