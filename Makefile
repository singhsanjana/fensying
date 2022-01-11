CDS_BASE_DIR := model-checker/cds-checker

all: basetests

basetests:
	$(MAKE) -C $(CDS_BASE_DIR)

clean:
	$(MAKE) -C $(CDS_BASE_DIR) clean