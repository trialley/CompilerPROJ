#include <CJsonObject.h>
#include <graph.h>

#include <fstream>
#include <string>
graph::graph() {
	_s.push(&_json);
	_root = &_json;

	//添加根部
	_root->Add("name", "root");
	_root->AddEmptySubArray("children");
	// _root = &((*_root)["children"]);
}  //初始化数据结构
void graph::graphAdd(const std::string name) {
	((*_root)["children"]).AddAsFirst(neb::CJsonObject());
	((*_root)["children"])[0].Add("name", name);
	((*_root)["children"])[0].AddEmptySubArray("children");
}
void graph::graphIn() {
	_s.push(_root);
	_root = &(((*_root)["children"])[0]["children"]);
}
void graph::graphOut() {
	_root = _s.top();
	_s.pop();
}
graph::~graph() {
	std::ofstream temp_node("./www/data.js");
	temp_node << "var data = ";
	temp_node << _json.ToString();
}  //输出图像