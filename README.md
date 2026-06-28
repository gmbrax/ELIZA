# ELIZA

A C implementation of the legendary ELIZA chatbot - one of the first programs capable of engaging in natural language
conversation with humans.

## What is ELIZA?

ELIZA is a groundbreaking computer program created by **Joseph Weizenbaum** at MIT between 1964 and 1966. It was one of
the first chatbots and an early example of primitive natural language processing.

### Historical Significance

- **First chatbot**: ELIZA demonstrated that machines could engage in seemingly intelligent conversation
- **Rogerian therapy simulation**: The most famous script, DOCTOR, mimics a Rogerian psychotherapist
- **The ELIZA effect**: Named after this program, it describes the tendency of humans to attribute human-like
  understanding to computer systems
- **Impact on AI**: ELIZA sparked debates about artificial intelligence, consciousness, and the nature of human-computer
  interaction

### How ELIZA Works

ELIZA operates using a surprisingly simple algorithm:

1. **Pattern Matching**: The program scans user input for keywords and patterns
2. **Decomposition**: It breaks down sentences based on these patterns
3. **Reassembly**: Using pre-defined rules, it transforms parts of the user's input into responses
4. **Reflection**: It often reflects the user's statements back as questions (e.g., "I am sad" → "Why are you sad?")
5. **Generic Responses**: When no pattern matches, it uses generic phrases like "Tell me more" or "How does that make
   you feel?"

Despite its simplicity, ELIZA can create an illusion of understanding that is surprisingly convincing!

## Prerequisites

Before building ELIZA, ensure you have the following installed:

- **CMake** version 3.12 or higher
    - Check version: `cmake --version`
    - Download from: https://cmake.org/download/

- **C Compiler** with C11 standard support:
    - **Linux**: GCC or Clang (usually pre-installed)
    - **macOS**: Clang (install via Xcode Command Line Tools: `xcode-select --install`)
    - **Windows**: MinGW-w64, Visual Studio, or Clang

## Building the Project

### Linux / macOS

1. **Clone or download the project**:
   ```bash
   git clone <repository-url>
   cd ELIZA
   ```

2. **Create a build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Generate build files**:
   ```bash
   cmake ..
   ```

4. **Compile the project**:
   ```bash
   cmake --build .
   ```
   Or simply:
   ```bash
   make
   ```

5. **The executable will be created in the build directory**

### Windows (Command Prompt / PowerShell)

1. **Clone or download the project**:
   ```cmd
   git clone <repository-url>
   cd ELIZA
   ```

2. **Create a build directory**:
   ```cmd
   mkdir build
   cd build
   ```

3. **Generate build files**:
   ```cmd
   cmake ..
   ```

   For Visual Studio:
   ```cmd
   cmake .. -G "Visual Studio 16 2019"
   ```

4. **Compile the project**:
   ```cmd
   cmake --build . --config Release
   ```

## Running ELIZA

### Linux / macOS

From the `build` directory:

```bash
./ELIZA
