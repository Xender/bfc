#include <cstdio>
#include <vector>

struct Op
{
	enum class Type
	{
		VAL,
		PTR,
		IN,
		OUT,
	} type;

	int repetitions;
};

class Program : public std::vector<Op>
{
public:
	void AppendOp(Op::Type type, int repetitions)
	{
		if(!empty() && back().type == type)
			back().repetitions += repetitions;
		else
			emplace_back( Op{type, repetitions} ); // Does emplacing like that gains us anything? (emplace_back(type, repetitions) gives an error, because there's no such ctor in POD).
	}
};

Program Parse(FILE* in)
{
	Program program;

	int c;
	while( (c = getc(in)) != EOF)
	{
		switch(c)
		{
			case '+': program.AppendOp(Op::Type::VAL,  1); break;
			case '-': program.AppendOp(Op::Type::VAL, -1); break;
			case '>': program.AppendOp(Op::Type::PTR,  1); break;
			case '<': program.AppendOp(Op::Type::PTR, -1); break;
			case '.': program.AppendOp(Op::Type::IN,   1); break;
			case ',': program.AppendOp(Op::Type::OUT,  1); break;
			case '[': /* TODO implement */ break;
			case ']': /* TODO implement */ break;

			default:;
		}
	}

	return program;
}

bool Validate(const Program& program)
{
	(void) program;
	return true;
}

namespace Opcodes {

const char* header = R"(
#include <stdio.h>
#define MEM_SIZE (16 * 1024)

int main()
{
	int mem[MEM_SIZE] = {};
	size_t idx = 0;

)";

const char* footer = R"(
	return 0;
}
)";

const char* val = R"(
	mem[idx] += %i;
)";

const char* ptr = R"(
	idx = (idx + %i) %% MEM_SIZE;
)";

const char* in = R"(
	mem[idx] = getchar();
)";

const char* out = R"(
	putchar(mem[idx]);
)";


} // namespace Opcodes

void EmitCode(FILE* out, const Program& program)
{
	void EmitOpcode(FILE* out, const Op& op);

	fprintf(out, Opcodes::header);

	for(const Op& op: program)
		EmitOpcode(out, op);

	fprintf(out, Opcodes::footer);
}

void EmitOpcode(FILE* out, const Op& op)
{
	switch(op.type)
	{
		case Op::Type::VAL:
			fprintf(out, Opcodes::val, op.repetitions);
			break;

		case Op::Type::PTR:
			fprintf(out, Opcodes::ptr, op.repetitions);
			break;

		case Op::Type::IN:
			break;

		case Op::Type::OUT:
			break;
	}
}

int main(/*int argc, char const *argv[]*/)
{
	Program program = Parse(stdin);

	if(Validate(program))
		EmitCode(stdout, program);

	return 0;
}
