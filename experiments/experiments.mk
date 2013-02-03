NO_AUTO_COMPILING = yes
EXPS := $(wildcard $(this_srcdir)*.exp)
BASES := $(notdir $(basename $(EXPS)))
RESULTS := $(addsuffix .results,$(BASES))
ESTABLISHED := $(foreach base,$(BASES),$(this_srcdir)$(base).established)
COMPARES := $(addsuffix .compare,$(BASES))

include $(this_srcdir)../common.mk

test: $(RESULTS)

compare: $(COMPARES)
	wc $(COMPARES)

reestablish: $(ESTABLISHED)

clean:
	rm -f *.results *.compare *.training *.model *.residuals *.full.*

define MAKE_RESULTS
$(1)_FULL_NUMS := $(addprefix $(1).full.,$(NUMS))

$$($(1)_FULL_NUMS): $(1).full.%: \
		$(this_srcdir)$(1).exp ../$(HANDS)_all $(this_srcdir)Makefile.local
	$(SIM) -a "`cat $$<`" -c "`awk '/^$$*/ {print $$$$2}' ../hands_all`" \
		-n $(NUM_TRIALS) -s "$$*" -f -g -t- -# > $$@
endef

$(RESULTS): %.results: $(this_srcdir)%.exp ../$(HANDS)_all
	$(SIM) -a "`cat $<`" -C ../$(HANDS)_all -n $(NUM_TRIALS) \
		-d- -t- -r- -u $(SUMMARY_FORMAT) > $@

$(foreach base,$(BASES),$(eval $(call MAKE_RESULTS,$(base))))

$(ESTABLISHED): $(this_srcdir)%.established: %.results
	cp $< $@

$(COMPARES): %.compare: %.results $(this_srcdir)../compare.awk
	awk -v results=$< -f $(this_srcdir)../compare.awk \
		$< $(this_srcdir)$*.established > $@

