# 获取当前目录下的所有 .cc 文件
SOURCES = $(wildcard *.cc)
# 生成与每个 .cc 文件对应的可执行文件
TARGETS = $(SOURCES:.cc=)

CC = g++ -L../src -g -O3 -Wall -std=c++17
LIBS = -lnetco
CFLAGS = -I../src

# 默认目标是生成所有可执行文件
all: $(TARGETS)

# 为每个 .cc 文件生成对应的可执行文件
%: %.cc
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# 清理生成的文件
clean:
	rm -rf $(TARGETS) *.o
