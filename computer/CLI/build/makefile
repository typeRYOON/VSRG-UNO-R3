# Build Invocation Command     ::
BUILD  = g++ -std=c++23 -O2 -Wall
NAME   = VSRG-UNO-R3

# Icon Resource Identifiers    ::
RES    = ..\..\..\res\icons\icon

# Include Identifiers          ::
INC-01 = ..\include\osuenv.cpp
INC-02 = ..\include\arduinohandler.cpp
INC-03 = ..\include\serial.cpp
INC-04 = ..\include\omaniaparser.cpp
INC-05 = ..\include\beatmap.cpp

# Final Build Files            ::
SRC    = ..\src\main.cpp
INC    = $(INC-01) $(INC-02) $(INC-03) $(INC-04) $(INC-05)

# Windows / UNIX command Check ::
ifeq ($(OS), Windows_NT)
	RM  = del /f
	CLS = cls
else
	RM  = rm -f
	CLS = clear
endif

# Build ::
build: clean
	@windres $(RES).rc -O coff -o $(RES).o
	@$(BUILD) $(SRC) $(INC) $(RES).o -o $(NAME).exe
	@echo ::
	@echo Success :: Built $(NAME) (CLI)!

# Clean ::
clean:
	@$(RM) $(NAME).exe
	@$(CLS)

# Run   ::
run:
	@.\$(NAME).exe