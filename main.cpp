#include <iostream>
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

Program Parse(std::istream& in)
{
	Program program;

	char c;
	while(in.get(c))
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
	return false;
}

namespace Codegen {

void EmitCode(std::ostream& out, const Program& program)
{
	extern const char* header;
	extern const char* footer;
	void EmitOpcode(std::ostream& out, const Op& op);

	out << header;

	for(const Op& op: program)
		EmitOpcode(out, op);

	out << footer;
}

void EmitOpcode(std::ostream& out, const Op& op)
{
	extern const char* val;
	extern const char* ptr;
	/*extern const char* in;
	extern const char* out______________________;*/

	switch(op.type)
	{
		case Op::Type::VAL:
			out << val;
			break;

		case Op::Type::PTR:
			out << ptr;
			break;

		case Op::Type::IN:
			break;

		case Op::Type::OUT:
			break;
	}
}

const char* header = R"(
#include <stdio.h>
#define MEM_SIZE (16 * 1024)

int main()
{
	int mem[MEM_SIZE];
	size_t idx;

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

/*const char* in = R"(

)";

const char* out______________________ = R"(

)";*/


} // namespace Codegen

using Codegen::EmitCode;

int main(/*int argc, char const *argv[]*/)
{
	Program program = Parse(std::cin);

	if(Validate(program))
		EmitCode(std::cout, program);

	return 0;
}
