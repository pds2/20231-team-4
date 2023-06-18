ifeq (,$(filter -j%,$(MAKEFLAGS)))
    MAKEFLAGS += -j$(shell nproc || echo 1)
endif
LIB = tmxlite box2d Candle-s sfml-graphics sfml-window sfml-system
IDIR = /usr/include /usr/local/include deps/include include
LDIR = deps/lib /usr/lib /usr/local/lib

ifneq (,$(SDIR))
	SRCS = $(shell find "$(SDIR)" -type f -name "*.cpp" | xargs grep -L "int main")
	FLAG += std=c++2a
	OBJS += $(SRCS:$(SDIR)/%.cpp=$(ODIR)/%.o)
	DEPS = $(OBJS:%.o=%.d)
	LINK = -Wl,-rpath='$$ORIGIN/lib'
	-include $(DEPS)
endif

define compile
	$(CXX) $1 $(addprefix -,$(FLAG)) $(addprefix -I,$(IDIR)) $(addprefix -L,$(LDIR)) $(addprefix -l,$(LIB))
endef
define submake
	FLAG="$1" SDIR="$2" ODIR="$3" OBJS="$5" EXE="$4" $(MAKE) --no-print-directory $3/$4
endef

all: debug release docs
test: debug
	@$(call submake,O0 g,tests,target/test,tester,$(wildcard target/debug/*.o))
	target/test/tester
flamegraph: debug
	perf record -g -o target/perf.data target/debug/main
	flamegraph --perfdata target/perf.data -o target/flamegraph.svg
debug:
	@$(call submake,O0 g,src,target/debug,main)
release:
	@$(call submake,O3,src,target/release,main)
docs:
	$(info Buiding docs to target/docs)
	@mkdir -p target/docs && doxygen .doxygen

$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR) deps
	$(info Compiling object $@)
	@$(call compile, -MMD -c $< -o $@)

$(ODIR)/$(EXE): $(SDIR)/$(EXE).cpp $(OBJS) | $(ODIR) deps
	$(info Compiling executable $@)
	@$(call compile, $^ -o $@ $(LINK))

$(ODIR):
	@mkdir -p $(ODIR)
	@ln -sf ../../deps/lib/ $(ODIR)/lib
deps:
	$(error Run ./configure first)
clean:
	$(info Cleaning targets)
	@$(RM) -r target

.PHONY: all test flamegraph debug release docs clean
