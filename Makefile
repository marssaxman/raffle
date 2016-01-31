
default: all
all: grammar raffle
raffle: bin/rfl
grammar: src/grammar.inl

include srctree.mk
-include $(call findtype, d, obj)

CXXFLAGS+=-MD -MP -Werror -g -fvisibility=hidden

bin/rfl: $(call cxx_objs, src, obj)
	@mkdir -p $(@D)
	g++ -o $@ $^
obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) -Isrc -std=c++11 $(CXXFLAGS) -c $< -o $@
obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -Isrc -std=c99 $(CXXFLAGS) -c $< -o $@

src/grammar.inl: src/grammar.peg
	peg $< -o $@

clean:
	-@rm -rf bin obj src/grammar.inl

.PHONY: all clean raffle grammar

