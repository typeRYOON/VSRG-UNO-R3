# Build Invocation Command       ::
BUILD = g++ -std=c++23 -O2 -Wall

# Solenoid Array Identifiers     ::
SA        = Solenoid-Array
SA-SRC-01 = main.cpp
SA-INC-01 = osuenv.cpp
SA-INC-02 = beatmap.cpp
SA-INC-03 = omaniaparser.cpp
SA-SRC    = src\$(SA)\$(SA-SRC-01)
SA-INC    = include\$(SA)\$(SA-INC-01) include\$(SA)\$(SA-INC-02) include\$(SA)\$(SA-INC-03)

# Keystrokes Display Identifiers ::
KS        = Keystrokes-Display
KS-SRC-01 = main.cpp
KS-INC-01 = osuenv.cpp
KS-INC-02 = keychecker.cpp
KS-SRC    = src\$(KS)\$(KS-SRC-01)
KS-INC    = include\$(KS)\$(KS-INC-01) include\$(KS)\$(KS-INC-02)

# KPS Display Identifiers        ::
KPS        = KPS-Display
KPS-SRC-01 = -1
KPS-INC-01 = -1
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

# Build Commands  ::
build-SA: clean # :: Solenoid Array.
	@$(BUILD) $(SA-SRC) $(SA-INC) -o build\$(SA).exe
	@echo ::
	@echo Success :: Built $(SA).exe !

build-KS: clean # :: Keystrokes Display.
	@$(BUILD) $(KS-SRC) $(KS-INC) -o build\$(KS).exe
	@echo ::
	@echo Success :: Built $(KS).exe !

build-KPS: clean # :: KPS Display.
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
