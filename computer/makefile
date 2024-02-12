# Build Invocation Command       ::
BUILD = g++ -std=c++23 -O2 -Wall

# Solenoid Array Identifiers     ::
SA-SRC-01 = main.cpp
SA-INC-01 = osuenv.cpp
SA        = Solenoid-Array
SA-SRC    = src\$(SA)\$(SA-SRC-01)
SA-INC    = include\$(SA)\$(SA-INC-01)

# Keystrokes Display Identifiers ::
KS-SRC-01 = -1
KS-INC-01 = -1
KS        = Keystrokes-Display
KS-SRC    = src\$(KS)\$(KS-SRC-01)
KS-INC    = include\$(KS)\$(KS-INC-01)

# KPS Display Identifiers        ::
KPS-SRC-01 = -1
KPS-INC-01 = -1
KPS        = KPS-Display
KPS-SRC    = src\$(KPS)\$(KPS-SRC-01)
KPS-INC    = include\$(KPS)\$(KPS-INC-01)

# Windows / UNIX command check   ::
ifeq ($(OS), Windows_NT)
	RM  = del /f
	CLS = cls
else
	RM  = rm -f
	CLS = clear
endif

# Build Commands ::
build-SA: clean #:: Solenoid Array.
	@$(BUILD) $(SA-SRC) $(SA-INC) -o build\$(SA).exe
	@echo ::
	@echo Success :: Built $(SA).exe !

build-KS: clean #:: Keystrokes Display.
	@$(BUILD) $(KS-SRC) -o build\$(KS).exe
	@echo ::
	@echo Success :: Built $(KS).exe !

build-KPS: clean #:: KPS Display.
	@$(BUILD) $(KPS-SRC) -o build\$(KPS).exe
	@echo ::
	@echo Success :: Built $(KPS).exe !

# Binary Cleaner ::
clean:
	@$(RM) build\$(SA).exe
	@$(RM) build\$(KS).exe
	@$(RM) build\$(KPS).exe
	@$(CLS)

# Running Specific Modules ::
run-SA: #:: Solenoid Array.
	@.\build\$(SA).exe

run-KS: #:: Keystrokes Display.
	@.\build\$(KS).exe

run-KPS: #:: KPS Display.
	@.\build\$(KPS).exe
