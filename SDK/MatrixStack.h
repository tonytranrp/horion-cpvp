#pragma once 
#include <stack>

class MatrixStack;

class MatrixStackRef {
private:
	MatrixStack* stack;
	glm::mat4x4& myPtr;

public:
	MatrixStackRef(MatrixStack* st, glm::mat4x4& top) : stack(st), myPtr(top){};
	~MatrixStackRef();

	auto& operator->() {
		return myPtr;
	}
	auto& operator*() {
		return myPtr;
	}
};

class MatrixStack {
private:
	std::stack<glm::mat4x4> stack;
	bool isDirty;

public:
	MatrixStackRef push() {
		this->isDirty = 1;
		const auto latestAndGreatest = this->stack.top();
		this->stack.emplace(latestAndGreatest);
		return MatrixStackRef(this, this->stack.top());
	}

	void pop() {
		this->isDirty = 1;
		this->stack.pop();
	}

	glm::mat4x4& top() {
		return this->stack.top();
	}
};