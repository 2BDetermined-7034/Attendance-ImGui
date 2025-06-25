# Attendance Style Guide

## Indentation and Braces
### Whitespace
- Use tabs instead of spaces for indentation. Configure them how you wish in your editor.
- Use `<NL>` instead of `<CR><NL>` for line separators.
- There should only ever by one indentation change per line.
### Functions
Function declarations follow the same style as classes and conditionals. Do not put the brace on the next line like K&R do.\
Do ✅
```
void func() {
    ...
}
```
Don't ❌
```
void func()
{
    ...
}
```
```
void
func()
{
    ...
}
```
### Conditionals
Add proper padding between braces, parenthesis, and keywords.\
Do ✅
```
if (condition) {
    ...
} else if (condition) {
    ...
} else {
    ...
}
```
Don't ❌
```
if(condition){
    ...
}else if(condition){
    ...
}else{ 
    ...
}
```
### Labels
Do **not** indent labels past the current indentation. This goes for switch statements, classes, and labels.\
Do ✅
```
class Foo {
public:
    ...
protected:
    ...
private:
    ...
};
```
```
switch (expression) {
case 0:
    ...
case 1:
    ...
default:
    ...
}
```
```
{
label:
    ...
}
```
Don't ❌
```
class Foo {
    public:
        ...
    protected:
        ...
    private:
        ...
};
```
```
switch (expression) {
    case 0:
        ...
    case 1:
        ...
    default:
        ...
}
```
```
{
    label:
        ...
}
```

### Operators
Add proper padding between operators and operands. We are not Python developers.\
Do ✅
```
a + b
a, b
for (auto i : array)
```
Don't ❌
```
a+b
a,b
for (auto i: array)
```

## Naming
### Code
| Category | Style | Example |
|-|-|-|
| Variables | `camalCase` | `U32 vertexCount;` |
| Functions | `camalCase` | `void writeAttributes();` |
| Constants | `camalCase` | `static constexpr U32 componentCount = 32;` |
| Enum fields | `camalCase` | `leftBracket = '['` |
| Template constants | `PascalCase` | `template <U32 Rows>` |
| Type | `PascalCase` | `class VertexArray` |
| Header guards | `UPPERCASE` | `VERTEXARRAY_HPP` |

Note: If an enum is exposed as an interface, be sure to make it an enum class instead so that it is accessed like `MyEnum::myConstant`. That way, there isn't just a `myConstant` identifier floating around.

### File structure
- Header files have a `.hpp` extension and live in the `inc/` directory.
- Source files have a `.cpp` extension and live in the `src/` directory.
- Files should contain one class and should be named after that class.
- Subdirectories separate classes and have a `.d` extension.
