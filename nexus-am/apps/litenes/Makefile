# NAME := litenes
# SRCS := $(shell find -L ./src/ -name "*.c")
# ROMS := $(shell find -L ./src/ -name "*.nes")
# PREBUILD := rom
# include $(AM_HOME)/Makefile.app

# .PHONY: rom
# rom: $(ROMS)
# 	@cd src/roms && python3 build-roms.py

NAME := litenes

# 1. 找出所有静态的 .c 文件，排除即将生成的 roms
SRCS := $(shell find -L ./src/ -name "*.c" -not -path "./src/roms/gen/*")

# 2. 根据 src/roms/rom/ 目录下的 .nes 文件，计算出将在 src/roms/gen/ 目录下生成的 .c 文件列表
ROMS_C := $(patsubst ./src/roms/rom/%.nes,./src/roms/gen/%.c,$(shell find -L ./src/roms/rom -name "*.nes"))

# 3. 将生成的 .c 文件列表加入到总的源文件列表中
SRCS += $(ROMS_C)

# 4. 包含 AM 的标准 Makefile 规则，并设置 PREBUILD 目标
PREBUILD := rom
include $(AM_HOME)/Makefile.app

# 5. 定义 rom 目标，用于生成文件
#    这个目标会先确保 gen 目录存在，然后运行 python 脚本
.PHONY: rom
rom:
	@mkdir -p src/roms/gen
	@cd src/roms && python3 build-roms.py

# 6. 定义依赖关系：告诉 make，任何一个 ROM C 文件都依赖于 rom 目标
#    这样当 make 发现 .c 文件不存在时，它知道要先执行 rom 目标来生成它
$(ROMS_C): rom