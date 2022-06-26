TARGET = ezmem.h

SHELL = /bin/bash

Q := @ # Silence commands, unsilence with Q=

SRCS = \
	init.h \
	utils_io.h \
	utils_file.h \
	utils_nbr.h \
	id_management.h \
	constructor.h \
	destructor.h \
	output_data.h \
	wrap_allo_free.h \

SRC = $(addprefix srcs/,$(SRCS))

VPATH := srcs/

all : rm header $(TARGET) footer
	$(Q)printf "\e[32;1m+++ CONCAT [$(SRC)] +++\e[0m\n"

.PHONY : rm header $(TARGET) footer
.FORCE : ;

rm : .FORCE
	$(Q)rm -f $(TARGET)

header :
	$(Q)printf "#ifndef EZMEM\n" >> $(TARGET)
	$(Q)printf " #define EZMEM\n" >> $(TARGET)

footer :
	$(Q)printf "\n#endif //EZMEM\n" >> $(TARGET)

$(TARGET) :
	$(Q)for file in $$(echo "$(SRC)" | xargs);\
	do \
	 	echo >> $(TARGET); \
		echo //////////////////////////////////////////////////////////// $$file >> $(TARGET); \
		cat $$file >> $(TARGET); \
	done


test : all
	gcc -g3 not_leaking.c && ./a.out

test_lk : all
	gcc -g3 leaking.c && ./a.out
