# Пути и переменные
BUILD := ./build
CMAKE := cmake

# Основные цели
.PHONY: all
all: debug

.PHONY: release debug
release debug:
	@$(CMAKE) --preset $@
	@$(CMAKE) --build $(BUILD) --target all -j $(shell nproc)

# Удаление сгенерированных файлов
.PHONY: clean
clean:
	@find ./ -name "build" -type d -exec rm -rf {} +
	@find . -name "*.user" -type f -delete