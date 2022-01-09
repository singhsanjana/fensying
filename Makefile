CDS_BASE_DIR := model-checker/cds-checker
TEST_DIR := test

all: basetests

basetests:
	$(MAKE) -C $(CDS_BASE_DIR)

clean:
	rm $(TEST_DIR)/*.o $(TEST_DIR)/*_fixed.cc $(TEST_DIR)/exec* $(TEST_DIR)/graph*