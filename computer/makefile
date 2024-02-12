# Build Command                  ::
BUILD = g++ -std=c++23 -O2 -Wall

# Solenoid Array Identifiers     ::
SRC-01  = Solenoid-Array
NAME-01 = main

# KPS Display Identifiers        ::
SRC-02  = KPS-Display
NAME-02 = main

# Keystrokes Display Identifiers ::
SRC-03  = Keystrokes-Display
NAME-03 = main

# Windows / UNIX command check   ::
ifeq ($(OS), Windows_NT)
	RM  = del /f
	CLS = cls
else
	RM  = rm -f
	CLS = clear
endif

# Build Commands ::
build-01: clean #:: Solenoid Array.
	@$(BUILD) src\$(SRC-01)\$(NAME-01).cpp -o build\$(SRC-01).exe
	@echo ::
	@echo Success :: Built $(SRC-01).exe !

build-02: clean #:: KPS Display.
	@$(BUILD) src\$(SRC-02)\$(NAME-02).cpp -o build\$(SRC-02).exe
	@echo ::
	@echo Success :: Built $(SRC-02).exe !

build-03: clean #:: Keystrokes Display.
	@$(BUILD) src\$(SRC-03)\$(NAME-03).cpp -o build\$(SRC-03).exe
	@echo ::
	@echo Success :: Built $(SRC-03).exe !

# Binary Cleaner ::
clean:
	@$(RM) build\$(SRC-01).exe
	@$(RM) build\$(SRC-02).exe
	@$(RM) build\$(SRC-03).exe
	@$(CLS)

# Running Specific Modules ::
run-01: #:: Solenoid Array.
	@.\build\$(SRC-01).exe

run-02: #:: KPS Display.
	@.\build\$(SRC-02).exe

run-03: #:: Keystrokes Display.
	@.\build\$(SRC-03).exe
