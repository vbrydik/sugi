# Define build directory and executable file name
BUILD_DIR="build"
EXEC_NAME="sugi"

# Create variable for executable file
EXEC_FILE="$(BUILD_DIR)/$(EXEC_NAME)"

build:
	@echo "Building $(EXEC_FILE)..."
	@mkdir -p build && cd build && cmake .. && make

run:
	@if [ ! -f "$(EXEC_FILE)" ]; then \
	    echo "$(EXEC_FILE) does not exist." ; \
	else \
	    echo "Running $(EXEC_FILE)..." ; \
	    $(EXEC_FILE) ; \
	fi

clear:
	@echo "Clear and remove $(BUILD_DIR)..."
	@rm -r $(BUILD_DIR)

