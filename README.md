# PasswordVault

## Objective

PasswordVault is a C-based command-line password manager designed to securely store user credentials in a local database. It introduces the concept of a **master password** (hashed and stored), supports credential management (add, view, reset), and uses hashing + encryption to protect stored passwords. The project demonstrates modular C programming, secure storage practices, and build automation with Makefiles.

### Skills Learned

* Writing modular **C code** with headers and implementation files.
* Implementing **SHA-256 hashing** and symmetric **encryption/decryption** for secure password storage.
* Designing and managing a **menu-driven CLI application**.
* Using **Makefiles** to build, clean, debug, and release applications.
* Working with external libraries: **json-c**, **OpenSSL (libssl, libcrypto)**.
* Implementing **secure user input handling** (masked passwords, input sanitization).
* Applying **file-based database management** to persist data between sessions.

### Tools Used

* **[C (GCC)](https://gcc.gnu.org/)** — primary implementation language.
* **[Make](https://www.gnu.org/software/make/)** — build automation (with debug, release, clean, install targets).
* **[OpenSSL](https://www.openssl.org/docs/)** (libssl, libcrypto) — cryptographic operations.
* **[json-c](https://json-c.github.io/json-c/)** — structured storage of credentials.
* **[cppcheck](http://cppcheck.sourceforge.net/)** (optional) — static code analysis.
* **[clang-format](https://clang.llvm.org/docs/ClangFormat.html)** (optional) — source code formatting.

## Steps

### 1) Clone and build the project

```bash
git clone https://github.com/LeviRichard/Password-manager.git
cd Password-manager

# build with default settings
make

# OR build in debug mode
make debug

# OR optimized release build
make release
```

> **Dependencies** (install before building on Ubuntu/Debian):

```bash
sudo apt update
sudo apt install build-essential libjson-c-dev libssl-dev
```

### 2) Run the application

```bash
./PasswordVault
```

### 3) Menu Options

When you run the program, you’ll see:

```
=================MENU=================
1. Create Master Password
2. View saved passwords
3. Add new password
4. Reset master password
5. Exit
--------------------------------------
Enter choice:
```

#### 1. Create Master Password

* Prompts you to set a **master password** and username.
* Master password is **hashed** before storage.
* If a master already exists, you’ll be prompted to reset before creating a new one.

#### 2. View Saved Passwords

* Lists all stored credentials (username, website, and encrypted password entries).

#### 3. Add New Password

* Requires the correct master password.
* Prompts for username, website, and password.
* Encrypts the password and stores it in the database.

#### 4. Reset Master Password

* Deletes the existing database.
* Requires confirmation (`Y/N`).
* Allows creating a new master and starting fresh.

#### 5. Exit

* Closes the program safely.

### 4) Cleaning and reinstalling

```bash
# remove build artifacts
make clean

# remove everything including backups
make distclean

# install globally (requires sudo)
make install

# uninstall globally
make uninstall
```



## Improving the Program

While PasswordVault demonstrates secure coding concepts, there are areas where the project could be expanded:

1. **Improve Input Security**

   * Replace `scanf` and `fflush(stdin)` with safer input functions.
   * Prevent buffer overflows by using bounded input everywhere.

2. **Enhance Database Security**

   * Currently stores hashed/encrypted data in files.
   * Could use a dedicated database engine (e.g., SQLite with encryption).

3. **Cross-Platform Portability**

   * Add Windows/Linux/macOS compatibility layers.
   * Improve terminal handling for password masking across platforms.

4. **Stronger Cryptography**

   * Replace fixed key usage with a key derivation function (KDF) like PBKDF2, bcrypt, or Argon2.
   * Use salt and iterations when hashing the master password.

5. **User Features**

   * Implement password generation (strong random passwords).
   * Add search/filter for stored entries.
   * Allow updating/deleting individual credentials.

6. **Codebase Quality**

   * Apply static analysis (`make analyze`) regularly.
   * Use `clang-format` to enforce consistent coding style.
   * Add **unit tests** for encryption, hashing, and database operations.

7. **Future Extensions**

   * Add a simple GUI (GTK/Qt) on top of the CLI.
   * Optionally sync with encrypted cloud storage.

