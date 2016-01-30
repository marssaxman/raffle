
default: all
all: raffle
raffle: bin/rfl

include srctree.mk
-include $(call findtype, d, obj)

CXXFLAGS+=-std=c++11 -MD -MP -Werror -g -fvisibility=hidden

bin/rfl: $(call cxx_objs, src, obj)
	@mkdir -p $(@D)
	g++ -o $@ $^
obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) -Isrc $(CXXFLAGS) -c $< -o $@

clean:
	-@rm -rf bin obj

.PHONY: all clean

