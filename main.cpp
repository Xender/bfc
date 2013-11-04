#include <cstdio>
#include <vector>

struct Op
{
	enum /*class*/ Type
	{
		VAL,
		PTR,
		IN,
		OUT,
		LOOP_BEG,
		LOOP_END,
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
			case '+': program.AppendOp(Op::Type::VAL,      1); break;
			case '-': program.AppendOp(Op::Type::VAL,     -1); break;
			case '>': program.AppendOp(Op::Type::PTR,      1); break;
			case '<': program.AppendOp(Op::Type::PTR,     -1); break;
			case '.': program.AppendOp(Op::Type::OUT,      1); break;
			case ',': program.AppendOp(Op::Type::IN,       1); break;
			case '[': program.AppendOp(Op::Type::LOOP_BEG, 1); break;
			case ']': program.AppendOp(Op::Type::LOOP_END, 1); break;

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
	char mem[MEM_SIZE] = {};
	size_t idx = 0;

)";

const char* footer = R"(
	return 0;
}
)";

const char* op[] =
{
	/* VAL,      */ "\tmem[idx] += %i;\n",
	/* PTR,      */ "\tidx = (idx + %i) %% MEM_SIZE;\n",
	/* IN,       */ "\tmem[idx] = getchar();\n",
	/* OUT,      */ "\tputchar(mem[idx]);\n",
	/* LOOP_BEG, */ "\twhile(mem[idx]) {\n" ,
	/* LOOP_END, */ "\t}\n" ,
};

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
		case Op::Type::PTR:
			fprintf(out, Opcodes::op[op.type], op.repetitions);
			break;

		case Op::Type::IN:
		case Op::Type::OUT:
		case Op::Type::LOOP_BEG:
		case Op::Type::LOOP_END:
			for(int i = 0; i < op.repetitions; ++i)
			fprintf(out, Opcodes::op[op.type]);
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
