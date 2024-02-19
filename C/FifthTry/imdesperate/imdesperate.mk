##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=imdesperate
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=D:/Studies/Machine-Oriented-Programming/Work/Machine-Oriented-Programming-Studies/C/FifthTry
ProjectPath            :=D:/Studies/Machine-Oriented-Programming/Work/Machine-Oriented-Programming-Studies/C/FifthTry/imdesperate
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/imdesperate
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=imme0
Date                   :=19/02/2024
CodeLitePath           :=D:/Studies/Machine-Oriented-Programming/CodeLite
MakeDirCommand         :=mkdir
LinkerName             :=D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-g++.exe
SharedObjectLinkerName :=D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=D:/Studies/Machine-Oriented-Programming/Work/Machine-Oriented-Programming-Studies/C/FifthTry/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  -T$(ProjectPath)/md407-ram.x -L$(ARM_V6LIB) -L$(ARM_GCC_V6LIB) -nostartfiles --specs=nano.specs
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)gcc $(LibrarySwitch)c_nano 
ArLibs                 :=  "gcc" "c_nano" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-ar.exe rcu
CXX      := D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-g++.exe
CC       := D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-gcc.exe
CXXFLAGS :=  -g -O0 -W $(Preprocessors)
CFLAGS   :=  -g -O0 -w -mthumb -march=armv6-m -mfloat-abi=soft -std=c99 --specs=nano.specs $(Preprocessors)
ASFLAGS  := 
AS       := D:/Studies/Machine-Oriented-Programming/Compilers/gcc-arm/bin/arm-none-eabi-as.exe


##
## User defined environment variables
##
CodeLiteDir:=D:\Studies\Machine-Oriented-Programming\CodeLite
TOOLDIR:=D:\Studies\Machine-Oriented-Programming\Compilers
ARM_GCC_VERSION:=12.3.1
ARM_V6LIB:=$(TOOLDIR)/gcc-arm/arm-none-eabi/lib/thumb/v6-m/nofp
ARM_GCC_V6LIB:=$(TOOLDIR)/gcc-arm/lib/gcc/arm-none-eabi/$(ARM_GCC_VERSION)/thumb/v6-m/nofp
ARM_M4SFPLIB:=$(TOOLDIR)/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/softfp
ARM_GCC_M4SFPLIB:=$(TOOLDIR)/gcc-arm/lib/gcc/arm-none-eabi/$(ARM_GCC_VERSION)/thumb/v7e-m+fp/softfp
ARM_M4HFPLIB:=$(TOOLDIR)/gcc-arm/arm-none-eabi/lib/thumb/v7e-m+fp/hard
ARM_GCC_M4HFPLIB:=$(TOOLDIR/gcc-arm/lib/gcc/arm-none-eabi/$(ARM_GCC_VERSION)/thumb/v7e-m+fp/hard
Objects0=$(IntermediateDirectory)/startup.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	$(TOOLDIR)/gcc-arm/bin/arm-none-eabi-objcopy -S -O srec  $(OutputFile) $(IntermediateDirectory)/$(ProjectName).s19
	$(TOOLDIR)/gcc-arm/bin/arm-none-eabi-objdump -D -S $(OutputFile) > $(IntermediateDirectory)/$(ProjectName).dass
	@echo Done

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/startup.c$(ObjectSuffix): startup.c $(IntermediateDirectory)/startup.c$(DependSuffix)
	$(CC) $(SourceSwitch) "D:/Studies/Machine-Oriented-Programming/Work/Machine-Oriented-Programming-Studies/C/FifthTry/imdesperate/startup.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/startup.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/startup.c$(DependSuffix): startup.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/startup.c$(ObjectSuffix) -MF$(IntermediateDirectory)/startup.c$(DependSuffix) -MM startup.c

$(IntermediateDirectory)/startup.c$(PreprocessSuffix): startup.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/startup.c$(PreprocessSuffix) startup.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


