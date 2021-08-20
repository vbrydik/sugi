BUILD_DIR="build"
EXAMPLES_DIR="examples"

build:
	@echo "Building..."
	@mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake .. && make

example:
	@if [ ! -f "$(BUILD_DIR)/$(EXAMPLES_DIR)/$(TARGET)" ]; then \
	    echo "$(BUILD_DIR)/$(EXAMPLES_DIR)/$(TARGET) DOES NOT exist!" ; \
	else \
	    echo "Running $(BUILD_DIR)/$(EXAMPLES_DIR)/$(TARGET)" ; \
	    $(BUILD_DIR)/$(EXAMPLES_DIR)/$(TARGET) ; \
	fi

# run:
# 	@if [ ! -f "$(EXEC_FILE)" ]; then \
# 	    echo "$(EXEC_FILE) does not exist." ; \
# 	else \
# 	    echo "Running $(EXEC_FILE)..." ; \
# 	    $(EXEC_FILE) ; \
# 	fi

clear:
	@echo "Clear and remove $(BUILD_DIR)..."
	@rm -r $(BUILD_DIR)

docs:
	@doxygen Doxyfile
