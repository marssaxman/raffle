
default: all
all: raffle
raffle: bin/rfl

include srctree.mk
-include $(call findtype, d, obj)

CXXFLAGS+=-MD -MP -Werror -g -fvisibility=hidden -Wswitch

bin/rfl: $(call cxx_objs, src, obj)
	@mkdir -p $(@D)
	g++ -o $@ $^
obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) -Isrc -std=c++11 $(CXXFLAGS) -c $< -o $@
obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -Isrc -std=c99 $(CXXFLAGS) -c $< -o $@

clean:
	-@rm -rf bin obj

.PHONY: all clean raffle

