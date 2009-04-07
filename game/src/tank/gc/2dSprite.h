// 2dSprite.h
/////////////////////////////////////////

#pragma once

#include "Actor.h"
#include "video/TextureManager.h" // TODO: try to remove

/////////////////////////////////////////////////

class TextureCache
{
	friend class GC_2dSprite;
	float  width;
	float  height;
	size_t texture;
	SpriteColor color;

public:
	TextureCache(const char *name);
};

///////////////////////////////////////////
// flags
#define GC_FLAG_2DSPRITE_VISIBLE               (GC_FLAG_ACTOR_ << 0)
#define GC_FLAG_2DSPRITE_INGRIDSET             (GC_FLAG_ACTOR_ << 1)
#define GC_FLAG_2DSPRITE_DROPSHADOW            (GC_FLAG_ACTOR_ << 2)
#define GC_FLAG_2DSPRITE_                      (GC_FLAG_ACTOR_ << 3)

class GC_2dSprite : public GC_Actor
{
	DECLARE_SELF_REGISTRATION(GC_2dSprite);

private:
	float  _rotation;

	SpriteColor  _color;
	size_t _texId;

private:
	unsigned int _frame;

public:
	inline size_t GetTexture() const { return _texId; }
	inline int   GetFrameCount()   const { return g_texman->Get(_texId).uvFrames.size(); }
	inline int   GetCurrentFrame() const { return _frame; }
	inline void  GetGlobalRect(FRECT &rect) const
	{
		const LogicalTexture &lt = g_texman->Get(_texId);
		rect.left   = GetPos().x - lt.pxFrameWidth * lt.uvPivot.x;
		rect.top    = GetPos().y - lt.pxFrameHeight * lt.uvPivot.y;
		rect.right  = rect.left + lt.pxFrameWidth;
		rect.bottom = rect.top  + lt.pxFrameHeight;
	}
	inline void  GetLocalRect(FRECT &rect) const
	{
		const LogicalTexture &lt = g_texman->Get(_texId);
		rect.left   = -lt.uvPivot.x * lt.pxFrameWidth;
		rect.top    = -lt.uvPivot.y * lt.pxFrameHeight;
		rect.right  = rect.left + lt.pxFrameWidth;
		rect.bottom = rect.top + lt.pxFrameHeight;
	}

	void SetTexture(const char *name);
	void SetTexture(const TextureCache &tc);

	void SetFrame(int nFrame);

	inline void SetSpriteRotation(float a) { _rotation = a; }
	inline void SetOpacity(float x) { SetOpacity1i( int(x * 255.0f) ); }
	inline void SetOpacity1i(int x) { _color.r = _color.g = _color.b = _color.a = x & 0xff; }
	inline void SetColor(BYTE r, BYTE g, BYTE b) { _color.r=r; _color.g=g; _color.b=b; }

	inline float GetSpriteRotation() const { return _rotation; }


	inline void SetShadow(bool bEnable)
	{
		SetFlags(GC_FLAG_2DSPRITE_DROPSHADOW, bEnable);
	}
	inline bool GetShadow() const
	{
		return CheckFlags(GC_FLAG_2DSPRITE_DROPSHADOW);
	}


private:
	ObjectList::iterator _globalZPos; // ������� � ������ onscreen ��� z_globals

private:
	void UpdateCurrentZ();
	void SetZ_current(enumZOrder z);
	enumZOrder _zOrderCurrent;
	enumZOrder _zOrderPrefered;

public:
	inline float GetSpriteWidth() const { return g_texman->Get(_texId).pxFrameWidth; }
	inline float GetSpriteHeight() const { return g_texman->Get(_texId).pxFrameHeight; }

public:
	void SetGridSet(bool bGridSet);
	void SetZ(enumZOrder z);
	enumZOrder GetZ() const;

	void SetVisible(bool bShow);
	inline bool GetVisible() const  { return CheckFlags(GC_FLAG_2DSPRITE_VISIBLE); }

public:
	GC_2dSprite();
	GC_2dSprite(FromFile);
	virtual ~GC_2dSprite();

	virtual void Serialize(SaveFile &f);
	virtual void Draw() const;
};

/////////////////////////////////////////////////////////////

class GC_UserSprite : public GC_2dSprite
{
	DECLARE_SELF_REGISTRATION(GC_UserSprite);

public:
	GC_UserSprite();
	GC_UserSprite(FromFile);

	virtual bool IsSaved() const { return true; }
};


///////////////////////////////////////////////////////////////////////////////
// end of file
