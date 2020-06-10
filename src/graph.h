#include <CJsonObject.h>

#include <stack>
#include <string>
class graph {
public:
	neb::CJsonObject _json;
	neb::CJsonObject* _root;
	std::stack<neb::CJsonObject*> _s;
	graph();  //初始化数据结构
	void graphAdd(std::string&&);
	void graphIn();
	void graphOut();
	~graph();  //输出图像
};