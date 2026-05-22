# options

A lightweight C++ library for parsing command-line arguments with validation support and flexible configuration options.

## Requirements

+ C++23 compatible compiler
+ CMake 3.11+

## Build

```bash
    git clone git@github.com:kcherno/options.git

    cd options

    cmake -B build -S .

    cmake --build build
```

## Quick start

<details>

<summary>basic usage</summary>

```c++
#include <exception>
#include <iostream>
#include <format>

#include "options/options.hpp"

int main(int argc, char** argv)
{
    try
    {
        options::grammar main_options {
            options::option 
            {
                .short_name  = "-h",
                .long_name   = "--help",
                .description = "print help message"
            }
        };

        main_options.description("main options:");

        const auto& [parsed_options, positional_options] =
            options::parser(main_options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: {} [<options>] [<arguments>]\n\n", argv[0]);

            std::cout << main_options << std::endl;
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;

        return 1;
    }
}
```

</details>

<details>

<summary>options with arguments</summary>

```c++
#include <exception>
#include <iostream>
#include <format>

#include "options/options.hpp"

int main(int argc, char** argv)
{
    try
    {
        options::grammar main_options {
            options::option {
                .short_name    = "-e",
                .long_name     = "--edit",
                .description   = "edit the file",
                .has_arguments = true
            },

            options::option {
                .long_name     = "--no-edit",
                .description   = "do not edit the file",
                .has_arguments = true
            },

            options::option {
                .short_name    = "-f",
                .long_name     = "--file",
                .description   = "add input file",
                .has_arguments = true
            },

            options::option 
            {
                .short_name  = "-h",
                .long_name   = "--help",
                .description = "print help message"
            }
        };

        main_options.description("main options:");

        const auto& [parsed_options, positional_options] =
            options::parser(main_options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: {} [<options>] [<arguments>]\n\n", argv[0]);

            std::cout << main_options << std::endl;
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;

        return 1;
    }
}
```

</details>

<details>

<summary>custom validation</summary>

```c++
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <format>

#include "options/options.hpp"

int main(int argc, char** argv)
{
    try
    {
        constexpr auto does_file_exist = [](std::string_view filename)
        {
            if (not std::filesystem::exists(filename))
            {
                throw std::runtime_error {
                    std::format(
                        "option::argument_verifier: the {} file doesn't exist",
                        filename)
                };
            }
        };

        options::grammar main_options {
            options::option {
                .short_name        = "-e",
                .long_name         = "--edit",
                .description       = "edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            options::option {
                .long_name         = "--no-edit",
                .description       = "do not edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            options::option {
                .short_name        = "-f",
                .long_name         = "--file",
                .description       = "add input file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            options::option 
            {
                .short_name  = "-h",
                .long_name   = "--help",
                .description = "print help message"
            }
        };

        main_options.description("main options:");

        const auto& [parsed_options, positional_options] =
            options::parser(main_options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: {} [<options>] [<arguments>]\n\n", argv[0]);

            std::cout << main_options << std::endl;
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;

        return 1;
    }
}
```

</details>

<details>

<summary>multiple grammars</summary>

```c++
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <format>

#include "options/options.hpp"

int main(int argc, char** argv)
{
    try
    {
        options::grammar main_options {
            options::option 
            {
                .short_name             = "-h",
                .long_name              = "--help",
                .description            = "print help message",
                .has_arguments          = true,
                .are_arguments_required = false,
                .argument_verifier      = [](std::string_view argument)
                {
                    if (argument != "filesystem")
                    {
                        throw std::runtime_error {
                            std::format(
                                "option::argument_verifier: "
                                "the {} is an invalid option group name\n"
                                "\n"
                                "list of options group:\n"
                                "\n"
                                "filesystem: working with files",
                                argument
                            )
                        };
                    }    
                }
            }
        };

        main_options.description("main options:");

        constexpr auto does_file_exist = [](std::string_view filename)
        {
            if (not std::filesystem::exists(filename))
            {
                throw std::runtime_error {
                    std::format(
                        "option::argument_verifier: the {} file doesn't exist",
                        filename)
                };
            }
        };

        options::grammar filesystem_options {
            options::option {
                .short_name        = "-e",
                .long_name         = "--edit",
                .description       = "edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            options::option {
                .long_name         = "--no-edit",
                .description       = "do not edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            options::option {
                .short_name        = "-f",
                .long_name         = "--file",
                .description       = "add input file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            }
        };

        filesystem_options.description("filesystem options:");

        const auto& [parsed_options, positional_options] =
            options::parser({main_options, filesystem_options})
                .parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            const auto& arguments = parsed_options["-h"];

            if (arguments.empty())
            {
                std::cout << std::format(
                    "usage: {} [<options>] [<arguments>]\n\n", argv[0]);
                
                std::cout << main_options << '\n'
                          << '\n'
                          << filesystem_options << '\n'
                          << '\n'
                          << "try using <help> followed by the name of "
                             "an option group to display only the options "
                             "of that group"
                          << std::endl;
            }

            else if (arguments.front() == "filesystem")
            {
                std::cout << filesystem_options << std::endl;
            }
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;

        return 1;
    }
}
```

</details>

## API documentation

### options::option

#### Types

| Type | Definition |
| --- | --- |
| `verifier_type` | `void(*)(std::string_view)` |

#### Data members

| Member | Type | Default initialization | Semantics |
| --- | :---: | :---: | --- |
| `short_name` | `std::string_view` | `{}` | Short option name, e.g., `-f` |
| `long_name` | `std::string_view` | `{}` | Long option name, e.g., `--file` |
| `description` | `std::string_view` | `{}` | Option description |
| `is_required` | `bool` | `false` | Specifies whether the option must be present in the command line |
| `has_arguments` | `bool` | `false` | Specifies whether the option has arguments |
| `are_arguments_required` | `bool` | `true` | Specifies whether the option must receive an argument on the command line |
| `argument_verifier` | `verifier_type` | `nullptr` | Function to validate a command line argument. ***The function must throw an exception on validation failure*** |

> [!TIP]
> *There is no need to initialize all fields. Use only what you need.*

#### Member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `representation()` | `std::string` | Returns the option representation for output, e.g., `-f, --file` |

### options::grammar

#### Static data members

Specifies the number of spaces on the left when outputting grammar.

```c++
    `inline static constinit std::size_t` left_indent = 4;
```

**Example:**

```c++
    ...

    grammar.left_indent = 0;

    std::cout << grammar << std::endl;

    // main options
    //
    // -h, --help print help message

    grammar.left_indent = 10;

    std::cout << grammar << std::endl;

    // main options
    //
    //           -h, --help print help message

    ...
```

Specifies the number of new lines after the description when outputting grammar.

```c++
    `inline static constinit std::size_t` vertical_gap = 1;
```

**Example:**

```c++
    ...

    grammar.vertical_gap = 0;

    std::cout << grammar << std::endl;

    // main options
    //     -h, --help print help message

    grammar.vertical_gap = 5;

    std::cout << grammar << std::endl;

    // main options
    //
    //
    //
    //
    //
    //           -h, --help print help message

    ...
```

#### Member functions

| Expression | Return Type | Semantics |
| --- | :---: | --- |
| `grammar()` | `grammar` | Default constructor. `grammar.empty()` is `true` |
| `grammar(std::string_view)` | `grammar` | Sets a description for a group of options. `grammar.empty()` is `true` |
| `grammar(std::initializer_list<option>)` | `grammar` | Initialize the grammar with a list of options |
| `grammar(const grammar&)` | `grammar` | Copy constructor |
| `grammar(grammar&&)` | `grammar` | Move constructor |
| `operator=(const grammar&)` | `grammar&` | Copy assignment operator |
| `operator=(grammar&&)` | `grammar&` | Move assignment operator |
| `add_option(const option&)` | `void` | Add the option to the grammar |
| `contains(std::string_view)` | `bool` | Checks whether an option is contained by short or long name |
| `description()` | `std::string_view` | Returns a description of a group of options |
| `description(std::string_view)` | `void` | Sets a new description for a group of options |
| `[static] verify(std::string_view)` | `void` | Verifies if the name matches the short or long option name pattern |
| `operator[](std::string_view)` | `const option&` | Returns a const reference to the option by its name |

### options::parser

#### Types

| Type | Definition |
| --- | --- |
| `parsed_options` | See below |
| `positional_options` | `std::vector<std::string_view>` |

#### Member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `parser()` | `parser` | Default constructor |
| `parser(const grammar&)` | `parser` | Initialize the parser with the grammar |
| `parser(std::initializer_list<grammar>)` | `parser` | Initialize the parser with a list of grammars |
| `parser(const parser&)` | `parser` | Copy constructor |
| `parser(parser&&)` | `parser` | Move constructor |
| `operator=(const parser&)` | `parser&` | Copy assignment operator |
| `operator=(parser&&)` | `parser&` | Move assignment operator |
| `parse_command_line(int, char**)` | `std::pair<parsed_options, positional_options>` | Parses command line and returns parsed options and positional options |
| `parse_command_line(int, const char**)` | `std::pair<parsed_options, positional_options>` | Parses command line and returns parsed options and positional options |

### options::parser::parsed_options

#### Member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `parsed_options(parsed_options&&)` | `parsed_options` | Move constructor |
| `operator=(parsed_options&&)` | `parsed_options` | Move assignment operator |
| `contains(std::string_view)` | `bool` | Checks whether an option is contained by short or long name |
| `operator[](std::string_view)` | `const std::vector<std::string_view>&` | Returns a list of arguments for the option |
