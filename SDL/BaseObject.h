#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "CommonFunction.h"

class BaseObject {
public:
	BaseObject();
	~BaseObject();
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; } // cai dat  vi tri
	SDL_Rect GetRect() const { return rect_; } // lay vi tri
	SDL_Texture* GetObject() const { return p_object_; }// lay bien luu anh

	bool LoadImg(std::string path, SDL_Renderer* screen);// load anh
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);// hien thi anh
	void Free();
protected:
	SDL_Texture* p_object_; // bien luu anh
	SDL_Rect rect_; // bien kich thuoc anh
};

#endif // !BASEOBJECT_H_
