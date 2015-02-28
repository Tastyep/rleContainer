NAME	= libRLE.a

COMP	= g++

RM	= rm -f

SRCDIR	= src/

INCDIR	= inc/

OBJDIR	= obj/

SRCS	=

OBJS	= $(addprefix $(OBJDIR), $(SRCS:.cpp=.o))

CFLAGS	= -O2 -march=native -std=c++11

LDFLAGS	= -lm -I $(INCDIR)

dummy	:= $(shell test -d $(OBJDIR) || mkdir $(OBJDIR))

$(OBJDIR)%.o:	$(SRCDIR)%.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
