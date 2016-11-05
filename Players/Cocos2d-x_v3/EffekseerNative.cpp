#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#include "EffekseerNative.h"
#include <math.h>
#include <float.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <fstream>
#include <memory>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif
﻿
#ifndef	__EFFEKSEER_BASE_H__
#define	__EFFEKSEER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
#define	EFK_STDCALL	__stdcall
#else
#define	EFK_STDCALL
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef uint16_t			EFK_CHAR;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Color;

class Manager;
class ManagerImplemented;
class Effect;
class EffectImplemented;
class EffectNode;
class EffectNodeRoot;
class EffectNodeSprite;
class EffectNodeRibbon;
class EffectNodeModel;

class InstanceGlobal;
class InstanceContainer;
class Instance;
class InstanceGroup;

class ParticleRenderer;
class SpriteRenderer;
class RibbonRenderer;
class ModelRenderer;
class TrackRenderer;

class Setting;
class FileReader;
class FileWriter;
class FileInterface;
class EffectLoader;
class TextureLoader;
class SoundLoader;
class ModelLoader;

class Model;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _DEBUG_EFFEKSEER
#define EffekseerPrintDebug(...)	printf(__VA_ARGS__)
#else
#define EffekseerPrintDebug(...)
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	インスタンスの状態
*/
enum eInstanceState
{
	/**
		@brief	正常動作中
	*/
	INSTANCE_STATE_ACTIVE,

	/**
		@brief	削除中
	*/
	INSTANCE_STATE_REMOVING,
	/**
		@brief	削除
	*/
	INSTANCE_STATE_REMOVED,

	INSTANCE_STATE_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	エフェクトに所属するノードの種類
*/
enum eEffectNodeType
{
	EFFECT_NODE_TYPE_ROOT = -1,
	EFFECT_NODE_TYPE_NONE = 0,
	EFFECT_NODE_TYPE_SPRITE = 2,
	EFFECT_NODE_TYPE_RIBBON = 3,
	EFFECT_NODE_TYPE_RING = 4,
	EFFECT_NODE_TYPE_MODEL = 5,
	EFFECT_NODE_TYPE_TRACK = 6,

	EFFECT_NODE_TYPE_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_BASE_H__﻿
#ifndef	__CULLING3D_CULLING3D_H__
#define	__CULLING3D_CULLING3D_H__



namespace Culling3D
{
	/**
	@brief	最大値取得
	*/
	template <typename T, typename U>
	T Max(T t, U u)
	{
		if (t > (T) u)
		{
			return t;
		}
		return u;
	}

	/**
	@brief	最小値取得
	*/
	template <typename T, typename U>
	T Min(T t, U u)
	{
		if (t < (T) u)
		{
			return t;
		}
		return u;
	}

	/**
	@brief	範囲内値取得
	*/
	template <typename T, typename U, typename V>
	T Clamp(T t, U max_, V min_)
	{
		if (t > (T) max_)
		{
			t = (T) max_;
		}

		if (t < (T) min_)
		{
			t = (T) min_;
		}

		return t;
	}

	template <class T>
	void SafeAddRef(T& t)
	{
		if (t != NULL)
		{
			t->AddRef();
		}
	}

	template <class T>
	void SafeRelease(T& t)
	{
		if (t != NULL)
		{
			t->Release();
			t = NULL;
		}
	}

	template <class T>
	void SafeSubstitute(T& target, T& value)
	{
		SafeAddRef(value);
		SafeRelease(target);
		target = value;
	}

	template <typename T>
	inline void SafeDelete(T*& p)
	{
		if (p != NULL)
		{
			delete (p);
			(p) = NULL;
		}
	}

	template <typename T>
	inline void SafeDeleteArray(T*& p)
	{
		if (p != NULL)
		{
			delete [](p);
			(p) = NULL;
		}
	}

	class World;
	class Object;

	struct Vector3DF
	{
		float	X;
		float	Y;
		float	Z;

		Vector3DF();
		Vector3DF(float x, float y, float z);

		bool operator == (const Vector3DF& o);
		bool operator != (const Vector3DF& o);

		Vector3DF operator-();

		Vector3DF operator + (const Vector3DF& o) const;

		Vector3DF operator - (const Vector3DF& o) const;

		Vector3DF operator * (const Vector3DF& o) const;

		Vector3DF operator / (const Vector3DF& o) const;

		Vector3DF operator * (const float& o) const;

		Vector3DF operator / (const float& o) const;

		Vector3DF& operator += (const Vector3DF& o);

		Vector3DF& operator -= (const Vector3DF& o);

		Vector3DF& operator *= (const Vector3DF& o);

		Vector3DF& operator /= (const Vector3DF& o);

		Vector3DF& operator *= (const float& o);

		Vector3DF& operator /= (const float& o);

		/**
		@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y + Z * Z;
		}

		/**
		@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector3DF GetNormal()
		{
			float length = GetLength();
			return Vector3DF(X / length, Y / length, Z / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		/**
		@brief	内積を取得する。
		*/
		static float Dot(const Vector3DF& v1, const Vector3DF& v2);

		/**
		@brief	外積を取得する。
		@note
		右手系の場合、右手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
		*/
		static Vector3DF Cross(const Vector3DF& v1, const Vector3DF& v2);

		/**
		@brief	2点間の距離を取得する。
		*/
		static float Distance(const Vector3DF& v1, const Vector3DF& v2);
	};
	
	struct Matrix44
	{
		float	Values[4][4];

		Matrix44();
		Matrix44& SetInverted();
		Vector3DF Transform3D(const Vector3DF& in) const;

		/**
		@brief	カメラ行列(右手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
		*/
		Matrix44& SetLookAtRH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up);

		/**
		@brief	カメラ行列(左手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
		*/
		Matrix44& SetLookAtLH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up);

		/**
		@brief	射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
		*/
		Matrix44& SetPerspectiveFovRH(float ovY, float aspect, float zn, float zf);

		/**
		@brief	OpenGL用射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
		*/
		Matrix44& SetPerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf);

		/**
		@brief	射影行列(左手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
		*/
		Matrix44& SetPerspectiveFovLH(float ovY, float aspect, float zn, float zf);

		/**
		@brief	正射影行列(右手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
		*/
		Matrix44& SetOrthographicRH(float width, float height, float zn, float zf);

		/**
		@brief	正射影行列(左手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
		*/
		Matrix44& SetOrthographicLH(float width, float height, float zn, float zf);

		Matrix44 operator * (const Matrix44& right) const;

		Vector3DF operator*(const Vector3DF& right) const;

		/**
		@brief	乗算を行う。
		@param	o	出力先
		@param	in1	行列1
		@param	in2	行列2
		@return	出力先の参照
		*/
		static Matrix44& Mul(Matrix44& o, const Matrix44& in1, const Matrix44& in2);
	};

	enum eObjectShapeType
	{
		OBJECT_SHAPE_TYPE_NONE,
		OBJECT_SHAPE_TYPE_SPHERE,
		OBJECT_SHAPE_TYPE_CUBOID,
		OBJECT_SHAPE_TYPE_ALL,
	};


	class IReference
	{
	public:
		/**
		@brief	参照カウンタを加算する。
		@return	加算後の参照カウンタ
		*/
		virtual int AddRef() = 0;

		/**
		@brief	参照カウンタを取得する。
		@return	参照カウンタ
		*/
		virtual int GetRef() = 0;

		/**
		@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
		@return	減算後の参照カウンタ
		*/
		virtual int Release() = 0;
	};

	class World
		: public IReference
	{
	public:
		virtual void AddObject(Object* o) = 0;
		virtual void RemoveObject(Object* o) = 0;

		virtual void CastRay(Vector3DF from, Vector3DF to) = 0;

		virtual void Culling(const Matrix44& cameraProjMat, bool isOpenGL) = 0;
		virtual int32_t GetObjectCount() = 0;
		virtual Object* GetObject(int32_t index) = 0;

		virtual bool Reassign() = 0;

		virtual void Dump(const char* path, const Matrix44& cameraProjMat, bool isOpenGL) = 0;

		static World* Create(float xSize, float ySize, float zSize, int32_t layerCount);
	};

	class Object
		: public IReference
	{
	public:
		virtual Vector3DF GetPosition() = 0;
		virtual void SetPosition(Vector3DF pos) = 0;
		virtual void ChangeIntoAll() = 0;
		virtual void ChangeIntoSphere(float radius) = 0;
		virtual void ChangeIntoCuboid(Vector3DF size) = 0;

		virtual void* GetUserData() = 0;
		virtual void SetUserData(void* data) = 0;

		static Object* Create();
	};
}

#endif﻿
#pragma once



namespace Culling3D
{
	class ReferenceObject
		: public IReference
	{
	private:
		int32_t	m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int32_t AddRef();

		virtual int32_t GetRef();

		virtual int32_t Release();
	};
}

﻿
#pragma once



namespace Culling3D
{
	class Grid
	{
	private:
		std::vector<Object*>	objects;

	public:
		Grid();

		void AddObject(Object* o);

		void RemoveObject(Object* o);

		std::vector<Object*>& GetObjects() { return objects; }

		bool IsScanned;
	};
}﻿
#pragma once



namespace Culling3D
{
	class Layer
	{
	private:
		int32_t		gridXCount;
		int32_t		gridYCount;
		int32_t		gridZCount;

		float		offsetX;
		float		offsetY;
		float		offsetZ;


		float		gridSize;
		std::vector<Grid>	grids;

	public:
		Layer(int32_t gridXCount, int32_t gridYCount, int32_t gridZCount, float offsetX, float offsetY, float offsetZ, float gridSize);
		virtual ~Layer();

		bool AddObject(Object* o);

		bool RemoveObject(Object* o);

		void AddGrids(Vector3DF max_, Vector3DF min_, std::vector<Grid*>& grids_);

		int32_t GetGridXCount() { return gridXCount; }
		int32_t GetGridYCount() { return gridYCount; }
		int32_t GetGridZCount() { return gridZCount; }

		float GetOffsetX() { return offsetX; }
		float GetOffsetY() { return offsetY; }
		float GetOffsetZ() { return offsetZ; }

		float GetGridSize() { return gridSize; }
		std::vector<Grid>& GetGrids() { return grids; }
	};
}﻿
#pragma once


namespace Culling3D
{
	class ObjectInternal
		: public Object
		, public ReferenceObject
	{
	public:
		struct Status
		{
			Vector3DF	Position;

			union
			{
				struct
				{
					float Radius;
				} Sphere;

				struct
				{
					float X;
					float Y;
					float Z;
				} Cuboid;
			} Data;

			float		radius;
			eObjectShapeType	Type;

			void CalcRadius()
			{
				radius = 0.0f;
				if (Type == OBJECT_SHAPE_TYPE_NONE) radius = 0.0f;
				if (Type == OBJECT_SHAPE_TYPE_SPHERE) radius = Data.Sphere.Radius;
				if (Type == OBJECT_SHAPE_TYPE_CUBOID) radius = sqrt(Data.Cuboid.X * Data.Cuboid.X + Data.Cuboid.Y * Data.Cuboid.Y + Data.Cuboid.Z * Data.Cuboid.Z) / 2.0f;
			}

			float GetRadius()
			{
				return radius;
			}
		};

	private:
		void*		userData;
		World*		world;

		Status	currentStatus;
		Status	nextStatus;

	public:
		ObjectInternal();
		virtual ~ObjectInternal();

		Vector3DF GetPosition() override;
		void SetPosition(Vector3DF pos) override;

		void ChangeIntoAll() override;

		void ChangeIntoSphere(float radius) override;

		void ChangeIntoCuboid(Vector3DF size) override;

		void* GetUserData() override;
		void SetUserData(void* userData) override;

		void SetWorld(World* world);

		Status	GetCurrentStatus() { return currentStatus; }
		Status	GetNextStatus() { return nextStatus; }

		int32_t ObjectIndex;

		virtual int32_t GetRef() { return ReferenceObject::GetRef(); }
		virtual int32_t AddRef() { return ReferenceObject::AddRef(); }
		virtual int32_t Release() { return ReferenceObject::Release(); }
	};
}﻿
#pragma once




namespace Culling3D
{
	class WorldInternal
		: public World
		, public ReferenceObject
	{
	private:
		float xSize;
		float ySize;
		float zSize;

		float	gridSize;
		float	minGridSize;
		int32_t	layerCount;

		std::vector<Layer*>	layers;

		Grid	outofLayers;
		Grid	allLayers;

		std::vector<Object*> objs;

		std::vector<Grid*> grids;

		std::set<Object*>	containedObjects;

	public:
		WorldInternal(float xSize, float ySize, float zSize, int32_t layerCount);
		virtual ~WorldInternal();

		void AddObject(Object* o);
		void RemoveObject(Object* o);

		void AddObjectInternal(Object* o);
		void RemoveObjectInternal(Object* o);

		void CastRay(Vector3DF from, Vector3DF to) override;

		void Culling(const Matrix44& cameraProjMat, bool isOpenGL);

		bool Reassign() override;

		void Dump(const char* path, const Matrix44& cameraProjMat, bool isOpenGL);

		int32_t GetObjectCount() { return objs.size(); }
		Object* GetObject(int32_t index) { return objs[index]; }

		virtual int32_t GetRef() { return ReferenceObject::GetRef(); }
		virtual int32_t AddRef() { return ReferenceObject::AddRef(); }
		virtual int32_t Release() { return ReferenceObject::Release(); }
	};
}﻿

namespace Culling3D
{
	Grid::Grid()
	{
		IsScanned = false;
	}

	void Grid::AddObject(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;
		assert(o_->ObjectIndex == -1);

		objects.push_back(o_);
		o_->ObjectIndex = objects.size() - 1;
	}

	void Grid::RemoveObject(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;
		assert(o_->ObjectIndex != -1);

		if (objects.size() == 1)
		{
			objects.clear();
		}
		else if (objects.size() - 1 == o_->ObjectIndex)
		{
			objects.resize(objects.size() - 1);
		}
		else
		{
			ObjectInternal* moved = (ObjectInternal*) objects[objects.size() - 1];
			moved->ObjectIndex = o_->ObjectIndex;
			objects[o_->ObjectIndex] = moved;
			objects.resize(objects.size() - 1);
		}

		o_->ObjectIndex = -1;
	}
}﻿

namespace Culling3D
{
	Layer::Layer(int32_t gridXCount, int32_t gridYCount, int32_t gridZCount, float offsetX, float offsetY, float offsetZ, float gridSize)
	{
		this->gridXCount = gridXCount;
		this->gridYCount = gridYCount;
		this->gridZCount = gridZCount;
		this->offsetX = offsetX;
		this->offsetY = offsetY;
		this->offsetZ = offsetZ;
		this->gridSize = gridSize;

		grids.resize(this->gridXCount * this->gridYCount * this->gridZCount);
	}

	Layer::~Layer()
	{

	}

	bool Layer::AddObject(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;

		float x = o_->GetNextStatus().Position.X + offsetX;
		float y = o_->GetNextStatus().Position.Y + offsetY;
		float z = o_->GetNextStatus().Position.Z + offsetZ;

		int32_t xind = (int32_t)(x / gridSize);
		int32_t yind = (int32_t)(y / gridSize);
		int32_t zind = (int32_t)(z / gridSize);

		int32_t ind = xind + yind * this->gridXCount + zind * this->gridXCount * this->gridYCount;

		if (xind < 0 ||
			xind >= this->gridXCount ||
			yind < 0 ||
			yind >= this->gridYCount ||
			zind < 0 ||
			zind >= this->gridZCount) return false;

		if (ind < 0 || ind >= (int32_t)grids.size()) return false;

		grids[ind].AddObject(o);

		return true;
	}

	bool Layer::RemoveObject(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;

		float x = o_->GetCurrentStatus().Position.X + offsetX;
		float y = o_->GetCurrentStatus().Position.Y + offsetY;
		float z = o_->GetCurrentStatus().Position.Z + offsetZ;

		int32_t xind = (int32_t) (x / gridSize);
		int32_t yind = (int32_t) (y / gridSize);
		int32_t zind = (int32_t) (z / gridSize);

		int32_t ind = xind + yind * this->gridXCount + zind * this->gridXCount * this->gridYCount;

		if (xind < 0 ||
			xind >= this->gridXCount ||
			yind < 0 ||
			yind >= this->gridYCount ||
			zind < 0 ||
			zind >= this->gridZCount) return false;

		if (ind < 0 || ind >= (int32_t) grids.size()) return false;

		grids[ind].RemoveObject(o);

		return true;
	}

	void Layer::AddGrids(Vector3DF max_, Vector3DF min_, std::vector<Grid*>& grids_)
	{
		int32_t maxX = (int32_t) ((max_.X + offsetX) / gridSize) + 1;
		int32_t maxY = (int32_t) ((max_.Y + offsetY) / gridSize) + 1;
		int32_t maxZ = (int32_t) ((max_.Z + offsetZ) / gridSize) + 1;

		int32_t minX = (int32_t) ((min_.X + offsetX) / gridSize) - 1;
		int32_t minY = (int32_t) ((min_.Y + offsetY) / gridSize) - 1;
		int32_t minZ = (int32_t) ((min_.Z + offsetZ) / gridSize) - 1;

		maxX = Clamp(maxX, gridXCount - 1, 0);
		maxY = Clamp(maxY, gridYCount - 1, 0);
		maxZ = Clamp(maxZ, gridZCount - 1, 0);

		minX = Clamp(minX, gridXCount - 1, 0);
		minY = Clamp(minY, gridYCount - 1, 0);
		minZ = Clamp(minZ, gridZCount - 1, 0);

		for (int32_t z = minZ; z <= maxZ; z++)
		{
			for (int32_t y = minY; y <= maxY; y++)
			{
				for (int32_t x = minX; x <= maxX; x++)
				{
					int32_t ind = x + y * this->gridXCount + z * this->gridXCount * this->gridYCount;

					if (!grids[ind].IsScanned)
					{
						grids_.push_back(&grids[ind]);
						grids[ind].IsScanned = true;
					}
				}
			}
		}
	}
}﻿

namespace Culling3D
{
	Matrix44::Matrix44()
	{
		for (int32_t c = 0; c < 4; c++)
		{
			for (int32_t r = 0; r < 4; r++)
			{
				Values[c][r] = 0.0f;
			}
		}

		for (int32_t i = 0; i < 4; i++)
		{
			Values[i][i] = 1.0f;
		}
	}

	Matrix44& Matrix44::SetInverted()
	{
		float a11 = this->Values[0][0];
		float a12 = this->Values[0][1];
		float a13 = this->Values[0][2];
		float a14 = this->Values[0][3];
		float a21 = this->Values[1][0];
		float a22 = this->Values[1][1];
		float a23 = this->Values[1][2];
		float a24 = this->Values[1][3];
		float a31 = this->Values[2][0];
		float a32 = this->Values[2][1];
		float a33 = this->Values[2][2];
		float a34 = this->Values[2][3];
		float a41 = this->Values[3][0];
		float a42 = this->Values[3][1];
		float a43 = this->Values[3][2];
		float a44 = this->Values[3][3];

		/* 行列式の計算 */
		float b11 = +a22 * (a33 * a44 - a43 * a34) - a23 * (a32 * a44 - a42 * a34) + a24 * (a32 * a43 - a42 * a33);
		float b12 = -a12 * (a33 * a44 - a43 * a34) + a13 * (a32 * a44 - a42 * a34) - a14 * (a32 * a43 - a42 * a33);
		float b13 = +a12 * (a23 * a44 - a43 * a24) - a13 * (a22 * a44 - a42 * a24) + a14 * (a22 * a43 - a42 * a23);
		float b14 = -a12 * (a23 * a34 - a33 * a24) + a13 * (a22 * a34 - a32 * a24) - a14 * (a22 * a33 - a32 * a23);

		float b21 = -a21 * (a33 * a44 - a43 * a34) + a23 * (a31 * a44 - a41 * a34) - a24 * (a31 * a43 - a41 * a33);
		float b22 = +a11 * (a33 * a44 - a43 * a34) - a13 * (a31 * a44 - a41 * a34) + a14 * (a31 * a43 - a41 * a33);
		float b23 = -a11 * (a23 * a44 - a43 * a24) + a13 * (a21 * a44 - a41 * a24) - a14 * (a21 * a43 - a41 * a23);
		float b24 = +a11 * (a23 * a34 - a33 * a24) - a13 * (a21 * a34 - a31 * a24) + a14 * (a21 * a33 - a31 * a23);

		float b31 = +a21 * (a32 * a44 - a42 * a34) - a22 * (a31 * a44 - a41 * a34) + a24 * (a31 * a42 - a41 * a32);
		float b32 = -a11 * (a32 * a44 - a42 * a34) + a12 * (a31 * a44 - a41 * a34) - a14 * (a31 * a42 - a41 * a32);
		float b33 = +a11 * (a22 * a44 - a42 * a24) - a12 * (a21 * a44 - a41 * a24) + a14 * (a21 * a42 - a41 * a22);
		float b34 = -a11 * (a22 * a34 - a32 * a24) + a12 * (a21 * a34 - a31 * a24) - a14 * (a21 * a32 - a31 * a22);

		float b41 = -a21 * (a32 * a43 - a42 * a33) + a22 * (a31 * a43 - a41 * a33) - a23 * (a31 * a42 - a41 * a32);
		float b42 = +a11 * (a32 * a43 - a42 * a33) - a12 * (a31 * a43 - a41 * a33) + a13 * (a31 * a42 - a41 * a32);
		float b43 = -a11 * (a22 * a43 - a42 * a23) + a12 * (a21 * a43 - a41 * a23) - a13 * (a21 * a42 - a41 * a22);
		float b44 = +a11 * (a22 * a33 - a32 * a23) - a12 * (a21 * a33 - a31 * a23) + a13 * (a21 * a32 - a31 * a22);

		// 行列式の逆数をかける
		float Det = (a11 * b11) + (a12 * b21) + (a13 * b31) + (a14 * b41);
		if ((-FLT_MIN <= Det) && (Det <= +FLT_MIN))
		{
			return *this;
		}

		float InvDet = 1.0f / Det;

		Values[0][0] = b11 * InvDet;
		Values[0][1] = b12 * InvDet;
		Values[0][2] = b13 * InvDet;
		Values[0][3] = b14 * InvDet;
		Values[1][0] = b21 * InvDet;
		Values[1][1] = b22 * InvDet;
		Values[1][2] = b23 * InvDet;
		Values[1][3] = b24 * InvDet;
		Values[2][0] = b31 * InvDet;
		Values[2][1] = b32 * InvDet;
		Values[2][2] = b33 * InvDet;
		Values[2][3] = b34 * InvDet;
		Values[3][0] = b41 * InvDet;
		Values[3][1] = b42 * InvDet;
		Values[3][2] = b43 * InvDet;
		Values[3][3] = b44 * InvDet;

		return *this;
	}

	Matrix44& Matrix44::SetLookAtRH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up)
	{
		// F=正面、R=右方向、U=上方向
		Vector3DF F = (eye - at).GetNormal();
		Vector3DF R = Vector3DF::Cross(up, F).GetNormal();
		Vector3DF U = Vector3DF::Cross(F, R).GetNormal();

		Values[0][0] = R.X;
		Values[0][1] = R.Y;
		Values[0][2] = R.Z;
		Values[0][3] = 0.0f;

		Values[1][0] = U.X;
		Values[1][1] = U.Y;
		Values[1][2] = U.Z;
		Values[1][3] = 0.0f;

		Values[2][0] = F.X;
		Values[2][1] = F.Y;
		Values[2][2] = F.Z;
		Values[2][3] = 0.0f;

		Values[0][3] = -Vector3DF::Dot(R, eye);
		Values[1][3] = -Vector3DF::Dot(U, eye);
		Values[2][3] = -Vector3DF::Dot(F, eye);
		Values[3][3] = 1.0f;
		return *this;
	}

	Matrix44& Matrix44::SetLookAtLH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up)
	{
		// F=正面、R=右方向、U=上方向
		Vector3DF F = (at - eye).GetNormal();
		Vector3DF R = Vector3DF::Cross(up, F).GetNormal();
		Vector3DF U = Vector3DF::Cross(F, R).GetNormal();

		Values[0][0] = R.X;
		Values[0][1] = R.Y;
		Values[0][2] = R.Z;
		Values[0][3] = 0.0f;

		Values[1][0] = U.X;
		Values[1][1] = U.Y;
		Values[1][2] = U.Z;
		Values[1][3] = 0.0f;

		Values[2][0] = F.X;
		Values[2][1] = F.Y;
		Values[2][2] = F.Z;
		Values[2][3] = 0.0f;

		Values[0][3] = -Vector3DF::Dot(R, eye);
		Values[1][3] = -Vector3DF::Dot(U, eye);
		Values[2][3] = -Vector3DF::Dot(F, eye);
		Values[3][3] = 1.0f;
		return *this;
	}

	Matrix44& Matrix44::SetPerspectiveFovRH(float ovY, float aspect, float zn, float zf)
	{
		float yScale = 1 / tanf(ovY / 2);
		float xScale = yScale / aspect;

		Values[0][0] = xScale;
		Values[1][0] = 0;
		Values[2][0] = 0;
		Values[3][0] = 0;

		Values[0][1] = 0;
		Values[1][1] = yScale;
		Values[2][1] = 0;
		Values[3][1] = 0;

		Values[0][2] = 0;
		Values[1][2] = 0;
		Values[2][2] = zf / (zn - zf);
		Values[3][2] = -1;

		Values[0][3] = 0;
		Values[1][3] = 0;
		Values[2][3] = zn * zf / (zn - zf);
		Values[3][3] = 0;
		return *this;
	}

	Matrix44& Matrix44::SetPerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf)
	{
		float yScale = 1 / tanf(ovY / 2);
		float xScale = yScale / aspect;
		float dz = zf - zn;

		Values[0][0] = xScale;
		Values[1][0] = 0;
		Values[2][0] = 0;
		Values[3][0] = 0;

		Values[0][1] = 0;
		Values[1][1] = yScale;
		Values[2][1] = 0;
		Values[3][1] = 0;

		Values[0][2] = 0;
		Values[1][2] = 0;
		Values[2][2] = -(zf + zn) / dz;
		Values[3][2] = -1.0f;

		Values[0][3] = 0;
		Values[1][3] = 0;
		Values[2][3] = -2.0f * zn * zf / dz;
		Values[3][3] = 0.0f;

		return *this;
	}

	Matrix44& Matrix44::SetPerspectiveFovLH(float ovY, float aspect, float zn, float zf)
	{
		float yScale = 1 / tanf(ovY / 2);
		float xScale = yScale / aspect;

		Values[0][0] = xScale;
		Values[1][0] = 0;
		Values[2][0] = 0;
		Values[3][0] = 0;

		Values[0][1] = 0;
		Values[1][1] = yScale;
		Values[2][1] = 0;
		Values[3][1] = 0;

		Values[0][2] = 0;
		Values[1][2] = 0;
		Values[2][2] = zf / (zf - zn);
		Values[3][2] = 1;

		Values[0][3] = 0;
		Values[1][3] = 0;
		Values[2][3] = -zn * zf / (zf - zn);
		Values[3][3] = 0;
		return *this;
	}

	Matrix44& Matrix44::SetOrthographicRH(float width, float height, float zn, float zf)
	{
		Values[0][0] = 2 / width;
		Values[1][0] = 0;
		Values[2][0] = 0;
		Values[3][0] = 0;

		Values[0][1] = 0;
		Values[1][1] = 2 / height;
		Values[2][1] = 0;
		Values[3][1] = 0;

		Values[0][2] = 0;
		Values[1][2] = 0;
		Values[2][2] = 1 / (zn - zf);
		Values[3][2] = 0;

		Values[0][3] = 0;
		Values[1][3] = 0;
		Values[2][3] = zn / (zn - zf);
		Values[3][3] = 1;
		return *this;
	}

	Matrix44& Matrix44::SetOrthographicLH(float width, float height, float zn, float zf)
	{
		Values[0][0] = 2 / width;
		Values[1][0] = 0;
		Values[2][0] = 0;
		Values[3][0] = 0;

		Values[0][1] = 0;
		Values[1][1] = 2 / height;
		Values[2][1] = 0;
		Values[3][1] = 0;

		Values[0][2] = 0;
		Values[1][2] = 0;
		Values[2][2] = 1 / (zf - zn);
		Values[3][2] = 0;

		Values[0][3] = 0;
		Values[1][3] = 0;
		Values[2][3] = zn / (zn - zf);
		Values[3][3] = 1;
		return *this;
	}

	Vector3DF Matrix44::Transform3D(const Vector3DF& in) const
	{
		float values[4];

		for (int i = 0; i < 4; i++)
		{
			values[i] = 0;
			values[i] += in.X * Values[i][0];
			values[i] += in.Y * Values[i][1];
			values[i] += in.Z * Values[i][2];
			values[i] += Values[i][3];
		}

		Vector3DF o;
		o.X = values[0] / values[3];
		o.Y = values[1] / values[3];
		o.Z = values[2] / values[3];
		return o;
	}

	Matrix44 Matrix44::operator * (const Matrix44& right) const
	{
		Matrix44 o_;
		Mul(o_, *this, right);
		return o_;
	}

	Vector3DF Matrix44::operator * (const Vector3DF& right) const
	{
		return Transform3D(right);
	}

	Matrix44& Matrix44::Mul(Matrix44& o, const Matrix44& in1, const Matrix44& in2)
	{
		Matrix44 _in1 = in1;
		Matrix44 _in2 = in2;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float v = 0.0f;
				for (int k = 0; k < 4; k++)
				{
					v += _in1.Values[i][k] * _in2.Values[k][j];
				}
				o.Values[i][j] = v;
			}
		}
		return o;
	}
}﻿

namespace Culling3D
{
	Object* Object::Create()
	{
		return new ObjectInternal();
	}

	ObjectInternal::ObjectInternal()
		: userData(NULL)
		, world(NULL)
		, ObjectIndex(-1)
	{
		currentStatus.Position = Vector3DF();
		currentStatus.radius = 0.0f;
		currentStatus.Type = OBJECT_SHAPE_TYPE_NONE;

		nextStatus.Position = Vector3DF();
		nextStatus.radius = 0.0f;
		nextStatus.Type = OBJECT_SHAPE_TYPE_NONE;
	}

	ObjectInternal::~ObjectInternal()
	{
	}

	Vector3DF ObjectInternal::GetPosition()
	{
		return nextStatus.Position;
	}

	void ObjectInternal::SetPosition(Vector3DF pos)
	{
		nextStatus.Position = pos;

		if (world != NULL)
		{
			WorldInternal* w = (WorldInternal*) world;
			w->RemoveObjectInternal(this);
			w->AddObjectInternal(this);
		}

		currentStatus = nextStatus;
	}

	void ObjectInternal::ChangeIntoAll()
	{
		nextStatus.Type = OBJECT_SHAPE_TYPE_ALL;
		nextStatus.CalcRadius();

		if (world != NULL)
		{
			WorldInternal* w = (WorldInternal*) world;
			w->RemoveObjectInternal(this);
			w->AddObjectInternal(this);
		}

		currentStatus = nextStatus;
	}

	void ObjectInternal::ChangeIntoSphere(float radius)
	{
		nextStatus.Data.Sphere.Radius = radius;
		nextStatus.Type = OBJECT_SHAPE_TYPE_SPHERE;
		nextStatus.CalcRadius();

		if (world != NULL)
		{
			WorldInternal* w = (WorldInternal*) world;
			w->RemoveObjectInternal(this);
			w->AddObjectInternal(this);
		}

		currentStatus = nextStatus;
	}

	void ObjectInternal::ChangeIntoCuboid(Vector3DF size)
	{
		nextStatus.Data.Cuboid.X = size.X;
		nextStatus.Data.Cuboid.Y = size.Y;
		nextStatus.Data.Cuboid.Z = size.Z;
		nextStatus.Type = OBJECT_SHAPE_TYPE_CUBOID;
		nextStatus.CalcRadius();

		if (world != NULL)
		{
			WorldInternal* w = (WorldInternal*) world;
			w->RemoveObjectInternal(this);
			w->AddObjectInternal(this);
		}

		currentStatus = nextStatus;
	}

	void* ObjectInternal::GetUserData()
	{
		return userData;
	}

	void ObjectInternal::SetUserData(void* userData)
	{
		this->userData = userData;
	}


	void ObjectInternal::SetWorld(World* world)
	{
		this->world = world;
	}
}﻿

namespace Culling3D
{
	ReferenceObject::ReferenceObject()
		: m_reference(1)
	{
	}

	ReferenceObject::~ReferenceObject()
	{
	}

	int32_t ReferenceObject::AddRef()
	{
		m_reference++;
		return m_reference;
	}

	int32_t ReferenceObject::GetRef()
	{
		return m_reference;
	}

	int32_t ReferenceObject::Release()
	{
		assert(m_reference > 0);

		m_reference--;
		bool destroy = m_reference == 0;
		if (destroy)
		{
			delete this;
			return 0;
		}

		return m_reference;
	}
}﻿

namespace Culling3D
{
	Vector3DF::Vector3DF()
		: X(0)
		, Y(0)
		, Z(0)
	{
	}

	Vector3DF::Vector3DF(float x, float y, float z)
		: X(x)
		, Y(y)
		, Z(z)
	{
	}

	bool Vector3DF::operator == (const Vector3DF& o)
	{
		return X == o.X && Y == o.Y && Z == o.Z;
	}

	bool Vector3DF::operator != (const Vector3DF& o)
	{
		return !(X == o.X && Y == o.Y && Z == o.Z);
	}

	Vector3DF Vector3DF::operator-()
	{
		return Vector3DF(-X, -Y, -Z);
	}

	Vector3DF Vector3DF::operator + (const Vector3DF& o) const
	{
		return Vector3DF(X + o.X, Y + o.Y, Z + o.Z);
	}

	Vector3DF Vector3DF::operator - (const Vector3DF& o) const
	{
		return Vector3DF(X - o.X, Y - o.Y, Z - o.Z);
	}

	Vector3DF Vector3DF::operator * (const Vector3DF& o) const
	{
		return Vector3DF(X * o.X, Y * o.Y, Z * o.Z);
	}

	Vector3DF Vector3DF::operator / (const Vector3DF& o) const
	{
		return Vector3DF(X / o.X, Y / o.Y, Z / o.Z);
	}

	Vector3DF Vector3DF::operator * (const float& o) const
	{
		return Vector3DF(X * o, Y * o, Z * o);
	}

	Vector3DF Vector3DF::operator / (const float& o) const
	{
		return Vector3DF(X / o, Y / o, Z / o);
	}

	Vector3DF& Vector3DF::operator += (const Vector3DF& o)
	{
		X += o.X;
		Y += o.Y;
		Z += o.Z;
		return *this;
	}

	Vector3DF& Vector3DF::operator -= (const Vector3DF& o)
	{
		X -= o.X;
		Y -= o.Y;
		Z -= o.Z;
		return *this;
	}

	Vector3DF& Vector3DF::operator *= (const float& o)
	{
		X *= o;
		Y *= o;
		Z *= o;
		return *this;
	}

	Vector3DF& Vector3DF::operator /= (const float& o)
	{
		X /= o;
		Y /= o;
		Z /= o;
		return *this;
	}

	float Vector3DF::Dot(const Vector3DF& v1, const Vector3DF& v2)
	{
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
	}

	Vector3DF Vector3DF::Cross(const Vector3DF& v1, const Vector3DF& v2)
	{
		Vector3DF o;

		float x = v1.Y * v2.Z - v1.Z * v2.Y;
		float y = v1.Z * v2.X - v1.X * v2.Z;
		float z = v1.X * v2.Y - v1.Y * v2.X;
		o.X = x;
		o.Y = y;
		o.Z = z;
		return o;
	}

	float Vector3DF::Distance(const Vector3DF& v1, const Vector3DF& v2)
	{
		float dx = v1.X - v2.X;
		float dy = v1.Y - v2.Y;
		float dz = v1.Z - v2.Z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
}﻿


namespace Culling3D
{
	const int32_t viewCullingXDiv = 2;
	const int32_t viewCullingYDiv = 2;
	const int32_t viewCullingZDiv = 3;

	bool IsInView(Vector3DF position, float radius, Vector3DF facePositions[6], Vector3DF faceDir[6])
	{
		for (int32_t i = 0; i < 6; i++)
		{
			Vector3DF diff = position - facePositions[i];
			float distance = Vector3DF::Dot(diff, faceDir[i]);
		
			if (distance > radius) return false;
		}

		return true;
	}

	World* World::Create(float xSize, float ySize, float zSize, int32_t layerCount)
	{
		return new WorldInternal(xSize, ySize, zSize, layerCount);
	}

	WorldInternal::WorldInternal(float xSize, float ySize, float zSize, int32_t layerCount)
	{
		this->xSize = xSize;
		this->ySize = ySize;
		this->zSize = zSize;

		this->gridSize = Max(Max(this->xSize, this->ySize), this->zSize);
		
		this->layerCount = layerCount;

		layers.resize(this->layerCount);

		for (size_t i = 0; i < layers.size(); i++)
		{
			float gridSize_ = this->gridSize / powf(2.0f, (float)i);

			int32_t xCount = (int32_t) (this->xSize / gridSize_);
			int32_t yCount = (int32_t) (this->ySize / gridSize_);
			int32_t zCount = (int32_t) (this->zSize / gridSize_);

			if (xCount * gridSize_ < this->xSize) xCount++;
			if (yCount * gridSize_ < this->ySize) yCount++;
			if (zCount * gridSize_ < this->zSize) zCount++;

			layers[i] = new Layer(xCount, yCount, zCount, xSize / 2.0f, ySize / 2.0f, zSize / 2.0f, gridSize_);
			
			this->minGridSize = gridSize_;
		}
	}

	WorldInternal::~WorldInternal()
	{
		for (size_t i = 0; i < layers.size(); i++)
		{
			delete layers[i];
		}

		layers.clear();

		for (std::set<Object*>::iterator it = containedObjects.begin(); it != containedObjects.end(); it++)
		{
			(*it)->Release();
		}
	}

	void WorldInternal::AddObject(Object* o)
	{
		SafeAddRef(o);
		containedObjects.insert(o);
		AddObjectInternal(o);
	}

	void WorldInternal::RemoveObject(Object* o)
	{
		RemoveObjectInternal(o);
		containedObjects.erase(o);
		SafeRelease(o);
	}

	void WorldInternal::AddObjectInternal(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;

		if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_ALL)
		{
			allLayers.AddObject(o);
			o_->SetWorld(this);
			return;
		}

		float radius = o_->GetNextStatus().GetRadius();
		if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_NONE || radius <= minGridSize)
		{
			if (layers[layers.size() - 1]->AddObject(o))
			{
			}
			else
			{
				outofLayers.AddObject(o);
			}
			o_->SetWorld(this);
			return;
		}

		int32_t gridInd = (int32_t) (gridSize / (radius * 2.0f));

		if (gridInd * (radius * 2) < gridSize) gridInd++;

		int32_t ind = 1;
		bool found = false;
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (ind <= gridInd && gridInd < ind * 2)
			{
				if (layers[i]->AddObject(o))
				{
					((ObjectInternal*) o)->SetWorld(this);
					found = true;
				}
				else
				{
					break;
				}
			}

			ind *= 2;
		}

		if (!found)
		{
			((ObjectInternal*) o)->SetWorld(this);
			outofLayers.AddObject(o);
		}
	}

	void WorldInternal::RemoveObjectInternal(Object* o)
	{
		assert(o != NULL);

		ObjectInternal* o_ = (ObjectInternal*) o;

		if (o_->GetCurrentStatus().Type == OBJECT_SHAPE_TYPE_ALL)
		{
			allLayers.RemoveObject(o);
			o_->SetWorld(NULL);
			return;
		}

		float radius = o_->GetCurrentStatus().GetRadius();
		if (o_->GetCurrentStatus().Type == OBJECT_SHAPE_TYPE_NONE || radius <= minGridSize)
		{
			if (layers[layers.size() - 1]->RemoveObject(o))
			{
			}
			else
			{
				outofLayers.RemoveObject(o);
			}
			o_->SetWorld(NULL);
			return;
		}

		int32_t gridInd = (int32_t) (gridSize / (radius * 2.0f));

		if (gridInd * (radius * 2.0f) < gridSize) gridInd++;

		int32_t ind = 1;
		bool found = false;
		for (size_t i = 0; i < layers.size(); i++)
		{
			if (ind <= gridInd && gridInd < ind * 2)
			{
				if (layers[i]->RemoveObject(o))
				{
					((ObjectInternal*) o)->SetWorld(NULL);
					found = true;
				}
				else
				{
					break;
				}
			}

			ind *= 2;
		}

		if (!found)
		{
			((ObjectInternal*) o)->SetWorld(NULL);
			outofLayers.RemoveObject(o);
		}
	}

	void WorldInternal::CastRay(Vector3DF from, Vector3DF to)
	{
		objs.clear();

		Vector3DF aabb_max;
		Vector3DF aabb_min;

		aabb_max.X = Max(from.X, to.X);
		aabb_max.Y = Max(from.Y, to.Y);
		aabb_max.Z = Max(from.Z, to.Z);

		aabb_min.X = Min(from.X, to.X);
		aabb_min.Y = Min(from.Y, to.Y);
		aabb_min.Z = Min(from.Z, to.Z);

		/* 範囲内に含まれるグリッドを取得 */
		for (size_t i = 0; i < layers.size(); i++)
		{
			layers[i]->AddGrids(aabb_max, aabb_min, grids);
		}

		/* 外領域追加 */
		grids.push_back(&outofLayers);
		grids.push_back(&allLayers);

		/* グリッドからオブジェクト取得 */
		
		/* 初期計算 */
		auto ray_dir = (to - from);
		auto ray_len = ray_dir.GetLength();
		ray_dir.Normalize();

		for (size_t i = 0; i < grids.size(); i++)
		{
			for (size_t j = 0; j < grids[i]->GetObjects().size(); j++)
			{
				Object* o = grids[i]->GetObjects()[j];
				ObjectInternal* o_ = (ObjectInternal*) o;

				if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_ALL)
				{
					objs.push_back(o);
					continue;
				}

				// 球線分判定
				{
					auto radius = o_->GetNextStatus().GetRadius();
					auto pos = o_->GetNextStatus().Position;

					auto from2pos = pos - from;
					auto from2nearLen = Vector3DF::Dot(from2pos, ray_dir);
					auto pos2ray = from2pos - ray_dir * from2nearLen;

					if (pos2ray.GetLength() > radius) continue;
					if (from2nearLen < 0 || from2nearLen > ray_len) continue;
				}

				if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_SPHERE)
				{
					objs.push_back(o);
					continue;
				}

				// AABB判定
				// 参考：http://marupeke296.com/COL_3D_No18_LineAndAABB.html

				if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_CUBOID)
				{
					// 交差判定
					float p[3], d[3], min[3], max[3];
					auto pos = o_->GetCurrentStatus().Position;
					memcpy(p, &from, sizeof(Vector3DF));
					memcpy(d, &ray_dir, sizeof(Vector3DF));
					memcpy(min, &pos, sizeof(Vector3DF));
					memcpy(max, &pos, sizeof(Vector3DF));

					min[0] -= o_->GetNextStatus().Data.Cuboid.X / 2.0f;
					min[1] -= o_->GetNextStatus().Data.Cuboid.Y / 2.0f;
					min[2] -= o_->GetNextStatus().Data.Cuboid.Z / 2.0f;

					max[0] += o_->GetNextStatus().Data.Cuboid.X / 2.0f;
					max[1] += o_->GetNextStatus().Data.Cuboid.Y / 2.0f;
					max[2] += o_->GetNextStatus().Data.Cuboid.Z / 2.0f;

					float t = -FLT_MAX;
					float t_max = FLT_MAX;

					for (int i = 0; i < 3; ++i)
					{
						if (abs(d[i]) < FLT_EPSILON)
						{
							if (p[i] < min[i] || p[i] > max[i])
							{
								// 交差していない
								continue;
							}
						}
						else
						{
							// スラブとの距離を算出
							// t1が近スラブ、t2が遠スラブとの距離
							float odd = 1.0f / d[i];
							float t1 = (min[i] - p[i]) * odd;
							float t2 = (max[i] - p[i]) * odd;
							if (t1 > t2)
							{
								float tmp = t1; t1 = t2; t2 = tmp;
							}

							if (t1 > t) t = t1;
							if (t2 < t_max) t_max = t2;

							// スラブ交差チェック
							if (t >= t_max)
							{
								// 交差していない
								continue;
							}
						}
					}

					// 交差している
					if (0 <= t  && t <= ray_len)
					{
						objs.push_back(o);
						continue;
					}
				}
			}
		}

		/* 取得したグリッドを破棄 */
		for (size_t i = 0; i < grids.size(); i++)
		{
			grids[i]->IsScanned = false;
		}

		grids.clear();
	}

	template<typename T> bool isfinite_(T arg)
	{
	    return arg == arg && 
	           arg != std::numeric_limits<T>::infinity() &&
	           arg != -std::numeric_limits<T>::infinity();
	}

	void WorldInternal::Culling(const Matrix44& cameraProjMat, bool isOpenGL)
	{
		objs.clear();
	
		
		if (!isfinite_(cameraProjMat.Values[2][2]) &&
			cameraProjMat.Values[0][0] != 0.0f &&
			cameraProjMat.Values[1][1] != 0.0f)
		{
			Matrix44 cameraProjMatInv = cameraProjMat;
			cameraProjMatInv.SetInverted();

			float maxx = 1.0f;
			float minx = -1.0f;

			float maxy = 1.0f;
			float miny = -1.0f;

			float maxz = 1.0f;
			float minz = 0.0f;
			if (isOpenGL) minz = -1.0f;

			Vector3DF eyebox[8];

			eyebox[0 + 0] = Vector3DF(minx, miny, maxz);
			eyebox[1 + 0] = Vector3DF(maxx, miny, maxz);
			eyebox[2 + 0] = Vector3DF(minx, maxy, maxz);
			eyebox[3 + 0] = Vector3DF(maxx, maxy, maxz);

			eyebox[0 + 4] = Vector3DF(minx, miny, minz);
			eyebox[1 + 4] = Vector3DF(maxx, miny, minz);
			eyebox[2 + 4] = Vector3DF(minx, maxy, minz);
			eyebox[3 + 4] = Vector3DF(maxx, maxy, minz);

			for (int32_t i = 0; i < 8; i++)
			{
				eyebox[i] = cameraProjMatInv.Transform3D(eyebox[i]);
			}

			// 0-right 1-left 2-top 3-bottom 4-front 5-back
			Vector3DF facePositions[6];
			facePositions[0] = eyebox[5];
			facePositions[1] = eyebox[4];
			facePositions[2] = eyebox[6];
			facePositions[3] = eyebox[4];
			facePositions[4] = eyebox[4];
			facePositions[5] = eyebox[0];

			Vector3DF faceDir[6];
			faceDir[0] = Vector3DF::Cross(eyebox[1] - eyebox[5], eyebox[7] - eyebox[5]);
			faceDir[1] = Vector3DF::Cross(eyebox[6] - eyebox[4], eyebox[0] - eyebox[4]);

			faceDir[2] = Vector3DF::Cross(eyebox[7] - eyebox[6], eyebox[2] - eyebox[6]);
			faceDir[3] = Vector3DF::Cross(eyebox[0] - eyebox[4], eyebox[5] - eyebox[4]);

			faceDir[4] = Vector3DF::Cross(eyebox[5] - eyebox[4], eyebox[6] - eyebox[4]);
			faceDir[5] = Vector3DF::Cross(eyebox[2] - eyebox[0], eyebox[1] - eyebox[5]);

			for (int32_t i = 0; i < 6; i++)
			{
				faceDir[i].Normalize();
			}

			for (int32_t z = 0; z < viewCullingZDiv; z++)
			{
				for (int32_t y = 0; y < viewCullingYDiv; y++)
				{
					for (int32_t x = 0; x < viewCullingXDiv; x++)
					{
						Vector3DF eyebox_[8];

						float xsize = 1.0f / (float) viewCullingXDiv;
						float ysize = 1.0f / (float) viewCullingYDiv;
						float zsize = 1.0f / (float) viewCullingZDiv;

						for (int32_t e = 0; e < 8; e++)
						{
							float x_, y_, z_;
							if (e == 0){ x_ = xsize * x; y_ = ysize * y; z_ = zsize * z; }
							if (e == 1){ x_ = xsize * (x + 1); y_ = ysize * y; z_ = zsize * z; }
							if (e == 2){ x_ = xsize * x; y_ = ysize * (y + 1); z_ = zsize * z; }
							if (e == 3){ x_ = xsize * (x + 1); y_ = ysize * (y + 1); z_ = zsize * z; }
							if (e == 4){ x_ = xsize * x; y_ = ysize * y; z_ = zsize * (z + 1); }
							if (e == 5){ x_ = xsize * (x + 1); y_ = ysize * y; z_ = zsize * (z + 1); }
							if (e == 6){ x_ = xsize * x; y_ = ysize * (y + 1); z_ = zsize * (z + 1); }
							if (e == 7){ x_ = xsize * (x + 1); y_ = ysize * (y + 1); z_ = zsize * (z + 1); }

							Vector3DF yzMid[4];
							yzMid[0] = eyebox[0] * x_ + eyebox[1] * (1.0f - x_);
							yzMid[1] = eyebox[2] * x_ + eyebox[3] * (1.0f - x_);
							yzMid[2] = eyebox[4] * x_ + eyebox[5] * (1.0f - x_);
							yzMid[3] = eyebox[6] * x_ + eyebox[7] * (1.0f - x_);

							Vector3DF zMid[2];
							zMid[0] = yzMid[0] * y_ + yzMid[1] * (1.0f - y_);
							zMid[1] = yzMid[2] * y_ + yzMid[3] * (1.0f - y_);

							eyebox_[e] = zMid[0] * z_ + zMid[1] * (1.0f - z_);
						}



						Vector3DF max_(-FLT_MAX, -FLT_MAX, -FLT_MAX);
						Vector3DF min_(FLT_MAX, FLT_MAX, FLT_MAX);

						for (int32_t i = 0; i < 8; i++)
						{
							if (eyebox_[i].X > max_.X) max_.X = eyebox_[i].X;
							if (eyebox_[i].Y > max_.Y) max_.Y = eyebox_[i].Y;
							if (eyebox_[i].Z > max_.Z) max_.Z = eyebox_[i].Z;

							if (eyebox_[i].X < min_.X) min_.X = eyebox_[i].X;
							if (eyebox_[i].Y < min_.Y) min_.Y = eyebox_[i].Y;
							if (eyebox_[i].Z < min_.Z) min_.Z = eyebox_[i].Z;
						}

						/* 範囲内に含まれるグリッドを取得 */
						for (size_t i = 0; i < layers.size(); i++)
						{
							layers[i]->AddGrids(max_, min_, grids);
						}
					}
				}
			}

			/* 外領域追加 */
			grids.push_back(&outofLayers);
			grids.push_back(&allLayers);

			/* グリッドからオブジェクト取得 */
			for (size_t i = 0; i < grids.size(); i++)
			{
				for (size_t j = 0; j < grids[i]->GetObjects().size(); j++)
				{
					Object* o = grids[i]->GetObjects()[j];
					ObjectInternal* o_ = (ObjectInternal*) o;

					if (
						o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_ALL ||
						IsInView(o_->GetPosition(), o_->GetNextStatus().GetRadius(), facePositions, faceDir))
					{
						objs.push_back(o);
					}
				}
			}

			/* 取得したグリッドを破棄 */
			for (size_t i = 0; i < grids.size(); i++)
			{
				grids[i]->IsScanned = false;
			}

			grids.clear();
		}
		else
		{
			grids.push_back(&allLayers);

			/* グリッドからオブジェクト取得 */
			for (size_t i = 0; i < grids.size(); i++)
			{
				for (size_t j = 0; j < grids[i]->GetObjects().size(); j++)
				{
					Object* o = grids[i]->GetObjects()[j];
					ObjectInternal* o_ = (ObjectInternal*) o;

					if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_ALL)
					{
						objs.push_back(o);
					}
				}
			}

			/* 取得したグリッドを破棄 */
			for (size_t i = 0; i < grids.size(); i++)
			{
				grids[i]->IsScanned = false;
			}

			grids.clear();
		}
		
	}

	bool WorldInternal::Reassign()
	{
		/* 数が少ない */
		if (outofLayers.GetObjects().size() < 10) return false;

		objs.clear();

		for (size_t i = 0; i < layers.size(); i++)
		{
			delete layers[i];
		}

		layers.clear();
		outofLayers.GetObjects().clear();
		allLayers.GetObjects().clear();

		outofLayers.IsScanned = false;
		allLayers.IsScanned = false;

		for (auto& it : containedObjects)
		{
			auto o = (ObjectInternal*) (it);
			o->ObjectIndex = -1;
		}

		float xmin = FLT_MAX;
		float xmax = -FLT_MAX;
		float ymin = FLT_MAX;
		float ymax = -FLT_MAX;
		float zmin = FLT_MAX;
		float zmax = -FLT_MAX;

		for (auto& it : containedObjects)
		{
			ObjectInternal* o_ = (ObjectInternal*) it;
			if (o_->GetNextStatus().Type == OBJECT_SHAPE_TYPE_ALL) continue;

			if (xmin > o_->GetNextStatus().Position.X) xmin = o_->GetNextStatus().Position.X;
			if (xmax < o_->GetNextStatus().Position.X) xmax = o_->GetNextStatus().Position.X;
			if (ymin > o_->GetNextStatus().Position.Y) ymin = o_->GetNextStatus().Position.Y;
			if (ymax < o_->GetNextStatus().Position.Y) ymax = o_->GetNextStatus().Position.Y;
			if (zmin > o_->GetNextStatus().Position.Z) zmin = o_->GetNextStatus().Position.Z;
			if (zmax < o_->GetNextStatus().Position.Z) zmax = o_->GetNextStatus().Position.Z;

		}

		auto xlen = Max(abs(xmax), abs(xmin)) * 2.0f;
		auto ylen = Max(abs(ymax), abs(ymin)) * 2.0f;
		auto zlen = Max(abs(zmax), abs(zmin)) * 2.0f;

		WorldInternal(xlen, ylen, zlen, this->layerCount);

		for (auto& it: containedObjects)
		{
			ObjectInternal* o_ = (ObjectInternal*) (it);
			AddObjectInternal(o_);
		}
		return true;
	}

	void WorldInternal::Dump(const char* path, const Matrix44& cameraProjMat, bool isOpenGL)
	{
		std::ofstream ofs(path);

		/* カメラ情報出力 */
		Matrix44 cameraProjMatInv = cameraProjMat;
		cameraProjMatInv.SetInverted();

		float maxx = 1.0f;
		float minx = -1.0f;

		float maxy = 1.0f;
		float miny = -1.0f;

		float maxz = 1.0f;
		float minz = 0.0f;
		if (isOpenGL) minz = -1.0f;

		Vector3DF eyebox[8];

		eyebox[0 + 0] = Vector3DF(minx, miny, maxz);
		eyebox[1 + 0] = Vector3DF(maxx, miny, maxz);
		eyebox[2 + 0] = Vector3DF(minx, maxy, maxz);
		eyebox[3 + 0] = Vector3DF(maxx, maxy, maxz);

		eyebox[0 + 4] = Vector3DF(minx, miny, minz);
		eyebox[1 + 4] = Vector3DF(maxx, miny, minz);
		eyebox[2 + 4] = Vector3DF(minx, maxy, minz);
		eyebox[3 + 4] = Vector3DF(maxx, maxy, minz);

		for (int32_t i = 0; i < 8; i++)
		{
			eyebox[i] = cameraProjMatInv.Transform3D(eyebox[i]);
		}

		ofs << viewCullingXDiv << "," << viewCullingYDiv << "," << viewCullingZDiv << std::endl;
		for (int32_t i = 0; i < 8; i++)
		{
			ofs << eyebox[i].X << "," << eyebox[i].Y << "," << eyebox[i].Z << std::endl;
		}
		ofs << std::endl;

		for (int32_t z = 0; z < viewCullingZDiv; z++)
		{
			for (int32_t y = 0; y < viewCullingYDiv; y++)
			{
				for (int32_t x = 0; x < viewCullingXDiv; x++)
				{
					Vector3DF eyebox_[8];

					float xsize = 1.0f / (float) viewCullingXDiv;
					float ysize = 1.0f / (float) viewCullingYDiv;
					float zsize = 1.0f / (float) viewCullingZDiv;

					for (int32_t e = 0; e < 8; e++)
					{
						float x_, y_, z_;
						if (e == 0){ x_ = xsize * x; y_ = ysize * y; z_ = zsize * z; }
						if (e == 1){ x_ = xsize * (x + 1); y_ = ysize * y; z_ = zsize * z; }
						if (e == 2){ x_ = xsize * x; y_ = ysize * (y + 1); z_ = zsize * z; }
						if (e == 3){ x_ = xsize * (x + 1); y_ = ysize * (y + 1); z_ = zsize * z; }
						if (e == 4){ x_ = xsize * x; y_ = ysize * y; z_ = zsize * (z + 1); }
						if (e == 5){ x_ = xsize * (x + 1); y_ = ysize * y; z_ = zsize * (z + 1); }
						if (e == 6){ x_ = xsize * x; y_ = ysize * (y + 1); z_ = zsize * (z + 1); }
						if (e == 7){ x_ = xsize * (x + 1); y_ = ysize * (y + 1); z_ = zsize * (z + 1); }

						Vector3DF yzMid[4];
						yzMid[0] = eyebox[0] * x_ + eyebox[1] * (1.0f - x_);
						yzMid[1] = eyebox[2] * x_ + eyebox[3] * (1.0f - x_);
						yzMid[2] = eyebox[4] * x_ + eyebox[5] * (1.0f - x_);
						yzMid[3] = eyebox[6] * x_ + eyebox[7] * (1.0f - x_);

						Vector3DF zMid[2];
						zMid[0] = yzMid[0] * y_ + yzMid[1] * (1.0f - y_);
						zMid[1] = yzMid[2] * y_ + yzMid[3] * (1.0f - y_);

						eyebox_[e] = zMid[0] * z_ + zMid[1] * (1.0f - z_);
					}

					Vector3DF max_(-FLT_MAX, -FLT_MAX, -FLT_MAX);
					Vector3DF min_(FLT_MAX, FLT_MAX, FLT_MAX);

					for (int32_t i = 0; i < 8; i++)
					{
						if (eyebox_[i].X > max_.X) max_.X = eyebox_[i].X;
						if (eyebox_[i].Y > max_.Y) max_.Y = eyebox_[i].Y;
						if (eyebox_[i].Z > max_.Z) max_.Z = eyebox_[i].Z;

						if (eyebox_[i].X < min_.X) min_.X = eyebox_[i].X;
						if (eyebox_[i].Y < min_.Y) min_.Y = eyebox_[i].Y;
						if (eyebox_[i].Z < min_.Z) min_.Z = eyebox_[i].Z;
					}

					ofs << x << "," << y << "," << z << std::endl;
					for (int32_t i = 0; i < 8; i++)
					{
						ofs << eyebox_[i].X << "," << eyebox_[i].Y << "," << eyebox_[i].Z << std::endl;
					}
					ofs << max_.X << "," << max_.Y << "," << max_.Z << std::endl;
					ofs << min_.X << "," << min_.Y << "," << min_.Z << std::endl;
					ofs << std::endl;
				}
			}
		}

		ofs << std::endl;
	
		/* レイヤー情報 */
		ofs << layers.size() << std::endl;

		for (size_t i = 0; i < layers.size(); i++)
		{
			auto& layer = layers[i];
			ofs << layer->GetGridXCount() << "," << layer->GetGridYCount() << "," << layer->GetGridZCount() 
				<< "," << layer->GetOffsetX() << "," << layer->GetOffsetY() << "," << layer->GetOffsetZ() << "," << layer->GetGridSize() << std::endl;
		
			for (size_t j = 0; j < layer->GetGrids().size(); j++)
			{
				auto& grid = layer->GetGrids()[j];

				if (grid.GetObjects().size() > 0)
				{
					ofs << j << "," << grid.GetObjects().size() << std::endl;
				}
			}
		}

		Culling(cameraProjMat, isOpenGL);


	}
}﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector2D::Vector2D()
	: X		( 0.0f )
	, Y		( 0.0f )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector2D::Vector2D( float x, float y )
	: X		( x )
	, Y		( y )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector2D& Vector2D::operator+=( const Vector2D& value )
{
	X += value.X;
	Y += value.Y;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if defined(_M_X86) && defined(__x86__)
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
	
inline float Rsqrt(float x)
{
#if defined(_M_X86) && defined(__x86__)
	_mm_store_ss(&x, _mm_rsqrt_ss(_mm_load_ss(&x)));
	return x;
#else
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x * x);
	x = x * (1.5f - xhalf * x * x);
	return x;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D::Vector3D()
	: X		( 0.0f )
	, Y		( 0.0f )
	, Z		( 0.0f )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D::Vector3D( float x, float y, float z )
	: X		( x )
	, Y		( y )
	, Z		( z )
{

}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D Vector3D::operator + ( const Vector3D& o ) const
{
	return Vector3D( X + o.X, Y + o.Y, Z + o.Z );
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D Vector3D::operator - ( const Vector3D& o ) const
{
	return Vector3D( X - o.X, Y - o.Y, Z - o.Z );
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D Vector3D::operator * ( const float& o ) const
{
	return Vector3D( X * o, Y * o, Z * o );
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D Vector3D::operator / ( const float& o ) const
{
	return Vector3D( X / o, Y / o, Z / o );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Vector3D::Add( Vector3D* pOut, const Vector3D* pIn1, const Vector3D* pIn2 )
{
	float x = pIn1->X + pIn2->X;
	float y = pIn1->Y + pIn2->Y;
	float z = pIn1->Z + pIn2->Z;
	pOut->X = x;
	pOut->Y = y;
	pOut->Z = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D& Vector3D::Sub( Vector3D& o, const Vector3D& in1, const Vector3D& in2 )
{
	o.X = in1.X - in2.X;
	o.Y = in1.Y - in2.Y;
	o.Z = in1.Z - in2.Z;
	return o;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D& Vector3D::operator += ( const Vector3D& o )
{
	X += o.X;
	Y += o.Y;
	Z += o.Z;
	return *this;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D& Vector3D::operator -= ( const Vector3D& o )
{
	X -= o.X;
	Y -= o.Y;
	Z -= o.Z;
	return *this;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D& Vector3D::operator *= ( const float& o )
{
	X *= o;
	Y *= o;
	Z *= o;
	return *this;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Vector3D& Vector3D::operator /= ( const float& o )
{
	X /= o;
	Y /= o;
	Z /= o;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float Vector3D::Length( const Vector3D& in )
{
	return sqrt( in.X * in.X + in.Y * in.Y + in.Z * in.Z );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float Vector3D::LengthSq( const Vector3D& in )
{
	return in.X * in.X + in.Y * in.Y + in.Z * in.Z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float Vector3D::Dot( const Vector3D& in1, const Vector3D& in2 )
{
	return in1.X * in2.X + in1.Y * in2.Y + in1.Z * in2.Z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Vector3D::Normal( Vector3D& o, const Vector3D& in )
{
	float inv = Rsqrt( in.X * in.X + in.Y * in.Y + in.Z * in.Z );
	o.X = in.X * inv;
	o.Y = in.Y * inv;
	o.Z = in.Z * inv;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D& Vector3D::Cross( Vector3D& o, const Vector3D& in1, const Vector3D& in2 )
{
	float x = in1.Y * in2.Z - in1.Z * in2.Y;
	float y = in1.Z * in2.X - in1.X * in2.Z;
	float z = in1.X * in2.Y - in1.Y * in2.X;
	o.X = x;
	o.Y = y;
	o.Z = z;
	return o;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D& Vector3D::Transform( Vector3D& o, const Vector3D& in, const Matrix43& mat )
{
	float values[4];
	for( int i = 0; i < 3; i++ )
	{
		values[i] = 0;
		values[i] += in.X * mat.Value[0][i];
		values[i] += in.Y * mat.Value[1][i];
		values[i] += in.Z * mat.Value[2][i];
		values[i] += mat.Value[3][i];
	}
	o.X = values[0];
	o.Y = values[1];
	o.Z = values[2];
	return o;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D& Vector3D::Transform( Vector3D& o, const Vector3D& in, const Matrix44& mat )
{
	float values[3];

	for( int i = 0; i < 3; i++ )
	{
		values[i] = 0;
		values[i] += in.X * mat.Values[0][i];
		values[i] += in.Y * mat.Values[1][i];
		values[i] += in.Z * mat.Values[2][i];
		values[i] += mat.Values[3][i];
	}

	o.X = values[0];
	o.Y = values[1];
	o.Z = values[2];
	return o;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Color::Color()
	: R	( 255 )
	, G	( 255 )
	, B	( 255 )
	, A	( 255 )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
	: R	( r )
	, G	( g )
	, B	( b )
	, A	( a )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Color::Mul( Color& o, const Color& in1, const Color& in2 )
{
	o.R = (uint8_t)((float)in1.R * (float)in2.R / 255.0f);
	o.G = (uint8_t)((float)in1.G * (float)in2.G / 255.0f);
	o.B = (uint8_t)((float)in1.B * (float)in2.B / 255.0f);
	o.A = (uint8_t)((float)in1.A * (float)in2.A / 255.0f);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RectF::RectF()
	: X			( 0.0f )
	, Y			( 0.0f )
	, Width		( 1.0f )
	, Height	( 1.0f )
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RectF::RectF( float x, float y, float width, float height )
	: X			( x )
	, Y			( y )
	, Width		( width )
	, Height	( height )
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector2D RectF::Position() const
{
	return Vector2D( X, Y );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector2D RectF::Size() const
{
	return Vector2D( Width, Height );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Indentity()
{
	static const Matrix43 indentity = {{
		{1.0f, 0.0f, 0.0f}, 
		{0.0f, 1.0f, 0.0f}, 
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f}
	}};
	memcpy( Value, indentity.Value, sizeof(indentity) );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Scaling( float x, float y, float z )
{
	memset( Value, 0, sizeof(float) * 12 );
	Value[0][0] = x;
	Value[1][1] = y;
	Value[2][2] = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationX( float angle )
{
	float c, s;
	::Effekseer::SinCos( angle, s, c );

	Value[0][0] = 1.0f;
	Value[0][1] = 0.0f;
	Value[0][2] = 0.0f;

	Value[1][0] = 0.0f;
	Value[1][1] = c;
	Value[1][2] = s;

	Value[2][0] = 0.0f;
	Value[2][1] = -s;
	Value[2][2] = c;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationY( float angle )
{
	float c, s;
	::Effekseer::SinCos( angle, s, c );

	Value[0][0] = c;
	Value[0][1] = 0.0f;
	Value[0][2] = -s;

	Value[1][0] = 0.0f;
	Value[1][1] = 1.0f;
	Value[1][2] = 0.0f;

	Value[2][0] = s;
	Value[2][1] = 0.0f;
	Value[2][2] = c;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationZ( float angle )
{
	float c, s;
	::Effekseer::SinCos( angle, s, c );

	Value[0][0] = c;
	Value[0][1] = s;
	Value[0][2] = 0.0f;

	Value[1][0] = -s;
	Value[1][1] = c;
	Value[1][2] = 0.0f;

	Value[2][0] = 0.0f;
	Value[2][1] = 0.0f;
	Value[2][2] = 1;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationXYZ( float rx, float ry, float rz )
{
	float cx, sx, cy, sy, cz, sz;
	
	if( rx != 0.0f )	::Effekseer::SinCos( rx, sx, cx );
	else				sx = 0.0f, cx = 1.0f;
	if( ry != 0.0f )	::Effekseer::SinCos( ry, sy, cy );
	else				sy = 0.0f, cy = 1.0f;
	if( rz != 0.0f )	::Effekseer::SinCos( rz, sz, cz );
	else				sz = 0.0f, cz = 1.0f;

	Value[0][0] = cy * cz;
	Value[0][1] = cy * sz;
	Value[0][2] = -sy;

	Value[1][0] = sx * sy * -sz + cx * -sz;
	Value[1][1] = sx * sy *  sz + cx *  cz;
	Value[1][2] = sx * cy;

	Value[2][0] = cx * sy * cz + sx * sz;
	Value[2][1] = cx * sy * sz - sx * cz;
	Value[2][2] = cx * cy;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationZXY( float rz, float rx, float ry )
{
	float cx, sx, cy, sy, cz, sz;

	if( rx != 0.0f )	::Effekseer::SinCos( rx, sx, cx );
	else				sx = 0.0f, cx = 1.0f;
	if( ry != 0.0f )	::Effekseer::SinCos( ry, sy, cy );
	else				sy = 0.0f, cy = 1.0f;
	if( rz != 0.0f )	::Effekseer::SinCos( rz, sz, cz );
	else				sz = 0.0f, cz = 1.0f;
	
	Value[0][0] = cz * cy + sz * sx * sy;
	Value[0][1] = sz * cx;
	Value[0][2] = cz * -sy + sz * sx * cy;

	Value[1][0] = -sz * cy + cz * sx * sy;
	Value[1][1] = cz * cx;
	Value[1][2] = -sz * -sy + cz * sx * cy;

	Value[2][0] = cx * sy;
	Value[2][1] = -sx;
	Value[2][2] = cx * cy;
	
	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationAxis( const Vector3D& axis, float angle )
{
	const float c = cosf( angle );
	const float s = sinf( angle );
	const float cc = 1.0f - c;

	Value[0][0] = cc * (axis.X * axis.X) + c;
	Value[0][1] = cc * (axis.X * axis.Y) + (axis.Z * s);
	Value[0][2] = cc * (axis.Z * axis.X) - (axis.Y * s);

	Value[1][0] = cc * (axis.X * axis.Y) - (axis.Z * s);
	Value[1][1] = cc * (axis.Y * axis.Y) + c;
	Value[1][2] = cc * (axis.Y * axis.Z) + (axis.X * s);

	Value[2][0] = cc * (axis.Z * axis.X) + (axis.Y * s);
	Value[2][1] = cc * (axis.Y * axis.Z) - (axis.X * s);
	Value[2][2] = cc * (axis.Z * axis.Z) + c;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::RotationAxis( const Vector3D& axis, float s, float c )
{
	const float cc = 1.0f - c;

	Value[0][0] = cc * (axis.X * axis.X) + c;
	Value[0][1] = cc * (axis.X * axis.Y) + (axis.Z * s);
	Value[0][2] = cc * (axis.Z * axis.X) - (axis.Y * s);

	Value[1][0] = cc * (axis.X * axis.Y) - (axis.Z * s);
	Value[1][1] = cc * (axis.Y * axis.Y) + c;
	Value[1][2] = cc * (axis.Y * axis.Z) + (axis.X * s);

	Value[2][0] = cc * (axis.Z * axis.X) + (axis.Y * s);
	Value[2][1] = cc * (axis.Y * axis.Z) - (axis.X * s);
	Value[2][2] = cc * (axis.Z * axis.Z) + c;

	Value[3][0] = 0.0f;
	Value[3][1] = 0.0f;
	Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Translation( float x, float y, float z )
{
	Indentity();
	Value[3][0] = x;
	Value[3][1] = y;
	Value[3][2] = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetSRT( Vector3D& s, Matrix43& r, Vector3D& t ) const
{
	t.X = Value[3][0];
	t.Y = Value[3][1];
	t.Z = Value[3][2];
	
	float sc[3];
	for( int m = 0; m < 3; m++ )
	{
		sc[m] = sqrt( Value[m][0] * Value[m][0] + Value[m][1] * Value[m][1] + Value[m][2] * Value[m][2] );
	}
	
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
	
	for( int m = 0; m < 3; m++ )
	{
		for( int n = 0; n < 3; n++ )
		{
			r.Value[m][n] = Value[m][n] / sc[m];
		}
	}
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetScale( Vector3D& s ) const
{
	float sc[3];
	for( int m = 0; m < 3; m++ )
	{
		sc[m] = sqrt( Value[m][0] * Value[m][0] + Value[m][1] * Value[m][1] + Value[m][2] * Value[m][2] );
	}
	
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetRotation( Matrix43& r ) const
{
	float sc[3];
	for( int m = 0; m < 3; m++ )
	{
		sc[m] = sqrt( Value[m][0] * Value[m][0] + Value[m][1] * Value[m][1] + Value[m][2] * Value[m][2] );
	}

	for( int m = 0; m < 3; m++ )
	{
		for( int n = 0; n < 3; n++ )
		{
			r.Value[m][n] = Value[m][n] / sc[m];
		}
	}
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetTranslation( Vector3D& t ) const
{
	t.X = Value[3][0];
	t.Y = Value[3][1];
	t.Z = Value[3][2];
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::SetSRT( const Vector3D& s, const Matrix43& r, const Vector3D& t )
{
	Matrix43 mats;
	mats.Scaling( s.X, s.Y, s.Z );

	Multiple( *this, mats, r );

	Value[3][0] = t.X;
	Value[3][1] = t.Y;
	Value[3][2] = t.Z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Multiple( Matrix43& out, const Matrix43& in1, const Matrix43& in2 )
{
#if 1
	Matrix43 temp1, temp2;
	// 共通の場合は一時変数にコピー
	const Matrix43& s1 = (&out == &in1) ? (temp1 = in1) : in1;
	const Matrix43& s2 = (&out == &in2) ? (temp2 = in2) : in2;
	
	out.Value[0][0] = s1.Value[0][0] * s2.Value[0][0] + s1.Value[0][1] * s2.Value[1][0] + s1.Value[0][2] * s2.Value[2][0];
	out.Value[0][1] = s1.Value[0][0] * s2.Value[0][1] + s1.Value[0][1] * s2.Value[1][1] + s1.Value[0][2] * s2.Value[2][1];
	out.Value[0][2] = s1.Value[0][0] * s2.Value[0][2] + s1.Value[0][1] * s2.Value[1][2] + s1.Value[0][2] * s2.Value[2][2];
									   					 				 				   				   
	out.Value[1][0] = s1.Value[1][0] * s2.Value[0][0] + s1.Value[1][1] * s2.Value[1][0] + s1.Value[1][2] * s2.Value[2][0];
	out.Value[1][1] = s1.Value[1][0] * s2.Value[0][1] + s1.Value[1][1] * s2.Value[1][1] + s1.Value[1][2] * s2.Value[2][1];
	out.Value[1][2] = s1.Value[1][0] * s2.Value[0][2] + s1.Value[1][1] * s2.Value[1][2] + s1.Value[1][2] * s2.Value[2][2];
									   					 				 				   				   
	out.Value[2][0] = s1.Value[2][0] * s2.Value[0][0] + s1.Value[2][1] * s2.Value[1][0] + s1.Value[2][2] * s2.Value[2][0];
	out.Value[2][1] = s1.Value[2][0] * s2.Value[0][1] + s1.Value[2][1] * s2.Value[1][1] + s1.Value[2][2] * s2.Value[2][1];
	out.Value[2][2] = s1.Value[2][0] * s2.Value[0][2] + s1.Value[2][1] * s2.Value[1][2] + s1.Value[2][2] * s2.Value[2][2];
									   					 				 				   				   
	out.Value[3][0] = s1.Value[3][0] * s2.Value[0][0] + s1.Value[3][1] * s2.Value[1][0] + s1.Value[3][2] * s2.Value[2][0] + s2.Value[3][0];
	out.Value[3][1] = s1.Value[3][0] * s2.Value[0][1] + s1.Value[3][1] * s2.Value[1][1] + s1.Value[3][2] * s2.Value[2][1] + s2.Value[3][1];
	out.Value[3][2] = s1.Value[3][0] * s2.Value[0][2] + s1.Value[3][1] * s2.Value[1][2] + s1.Value[3][2] * s2.Value[2][2] + s2.Value[3][2];
#else
	Matrix43 temp;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			float v = 0.0f;
			for( int k = 0; k < 3; k++ )
			{
				v += in1.Value[i][k] * in2.Value[k][j];
			}
			temp.Value[i][j] = v;
		}
	}

	for( int i = 0; i < 3; i++ )
	{
		temp.Value[3][i] += in2.Value[3][i];
	}

	out = temp;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44::Matrix44()
{
	Indentity();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::Indentity()
{
	memset( Values, 0, sizeof(float) * 16 );
	Values[0][0] = 1.0f;
	Values[1][1] = 1.0f;
	Values[2][2] = 1.0f;
	Values[3][3] = 1.0f;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::Transpose()
{
	for (int32_t c = 0; c < 4; c++)
	{
		for (int32_t r = c; r < 4; r++)
		{
			float v = Values[r][c];
			Values[r][c] = Values[c][r];
			Values[c][r] = v;
		}
	}

	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::LookAtRH( const Vector3D& eye, const Vector3D& at, const Vector3D& up )
{
	// F=正面、R=右方向、U=上方向
	Vector3D F; 
	Vector3D R;
	Vector3D U;
	Vector3D::Normal( F, Vector3D::Sub( F, eye, at ) );
	Vector3D::Normal( R, Vector3D::Cross( R, up, F ) );
	Vector3D::Normal( U, Vector3D::Cross( U, F, R ) );

	Values[0][0] = R.X;
	Values[1][0] = R.Y;
	Values[2][0] = R.Z;
	Values[3][0] = 0.0f;

	Values[0][1] = U.X;
	Values[1][1] = U.Y;
	Values[2][1] = U.Z;
	Values[3][1] = 0.0f;

	Values[0][2] = F.X;
	Values[1][2] = F.Y;
	Values[2][2] = F.Z;
	Values[3][2] = 0.0f;

	Values[3][0] = - Vector3D::Dot( R, eye );
	Values[3][1] = - Vector3D::Dot( U, eye );
	Values[3][2] = - Vector3D::Dot( F, eye );
	Values[3][3] = 1.0f;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::LookAtLH( const Vector3D& eye, const Vector3D& at, const Vector3D& up )
{
	// F=正面、R=右方向、U=上方向
	Vector3D F; 
	Vector3D R;
	Vector3D U;
	Vector3D::Normal( F, Vector3D::Sub( F, at, eye ) );
	Vector3D::Normal( R, Vector3D::Cross( R, up, F ) );
	Vector3D::Normal( U, Vector3D::Cross( U, F, R ) );

	Values[0][0] = R.X;
	Values[1][0] = R.Y;
	Values[2][0] = R.Z;
	Values[3][0] = 0.0f;

	Values[0][1] = U.X;
	Values[1][1] = U.Y;
	Values[2][1] = U.Z;
	Values[3][1] = 0.0f;

	Values[0][2] = F.X;
	Values[1][2] = F.Y;
	Values[2][2] = F.Z;
	Values[3][2] = 0.0f;

	Values[3][0] = - Vector3D::Dot( R, eye );
	Values[3][1] = - Vector3D::Dot( U, eye );
	Values[3][2] = - Vector3D::Dot( F, eye );
	Values[3][3] = 1.0f;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::PerspectiveFovRH( float ovY, float aspect, float zn, float zf )
{
	float yScale = 1 / tanf( ovY / 2 );
	float xScale = yScale / aspect;

	Values[0][0] = xScale;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;

	Values[1][0] = 0;
	Values[1][1] = yScale;
	Values[1][2] = 0;
	Values[1][3] = 0;

	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = zf / (zn-zf);
	Values[2][3] = -1;

	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = zn * zf / (zn-zf);
	Values[3][3] = 0;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::PerspectiveFovRH_OpenGL( float ovY, float aspect, float zn, float zf )
{
	float yScale = 1 / tanf( ovY / 2 );
	float xScale = yScale / aspect;
	float dz = zf - zn;

	Values[0][0] = xScale;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;

	Values[1][0] = 0;
	Values[1][1] = yScale;
	Values[1][2] = 0;
	Values[1][3] = 0;

	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = -(zf + zn) / dz;
	Values[2][3] = -1.0f;

	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = -2.0f * zn * zf / dz;
	Values[3][3] = 0.0f;

	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::PerspectiveFovLH( float ovY, float aspect, float zn, float zf )
{
	float yScale = 1 / tanf( ovY / 2 );
	float xScale = yScale / aspect;

	Values[0][0] = xScale;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;

	Values[1][0] = 0;
	Values[1][1] = yScale;
	Values[1][2] = 0;
	Values[1][3] = 0;

	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = zf / (zf-zn);
	Values[2][3] = 1;

	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = -zn * zf / (zf-zn);
	Values[3][3] = 0;
	return *this;
}
	
//----------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------
Matrix44& Matrix44::PerspectiveFovLH_OpenGL( float ovY, float aspect, float zn, float zf )
{
	float yScale = 1 / tanf( ovY / 2 );
	float xScale = yScale / aspect;
		
	Values[0][0] = xScale;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;
		
	Values[1][0] = 0;
	Values[1][1] = yScale;
	Values[1][2] = 0;
	Values[1][3] = 0;
		
	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = zf / (zf-zn);
	Values[2][3] = 1;
		
	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = -2.0f * zn * zf / (zf-zn);
	Values[3][3] = 0;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::OrthographicRH( float width, float height, float zn, float zf )
{
	Values[0][0] = 2 / width;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;

	Values[1][0] = 0;
	Values[1][1] = 2 / height;
	Values[1][2] = 0;
	Values[1][3] = 0;

	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = 1 / (zn-zf);
	Values[2][3] = 0;

	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = zn / (zn-zf);
	Values[3][3] = 1;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::OrthographicLH( float width, float height, float zn, float zf )
{
	Values[0][0] = 2 / width;
	Values[0][1] = 0;
	Values[0][2] = 0;
	Values[0][3] = 0;

	Values[1][0] = 0;
	Values[1][1] = 2 / height;
	Values[1][2] = 0;
	Values[1][3] = 0;

	Values[2][0] = 0;
	Values[2][1] = 0;
	Values[2][2] = 1 / (zf-zn);
	Values[2][3] = 0;

	Values[3][0] = 0;
	Values[3][1] = 0;
	Values[3][2] = zn / (zn-zf);
	Values[3][3] = 1;
	return *this;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::Scaling( float x, float y, float z )
{
	memset( Values, 0, sizeof(float) * 16 );
	Values[0][0] = x;
	Values[1][1] = y;
	Values[2][2] = z;
	Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::RotationX( float angle )
{
	float c, s;
	SinCos( angle, s, c );

	Values[0][0] = 1.0f;
	Values[0][1] = 0.0f;
	Values[0][2] = 0.0f;
	Values[0][3] = 0.0f;

	Values[1][0] = 0.0f;
	Values[1][1] = c;
	Values[1][2] = s;
	Values[1][3] = 0.0f;

	Values[2][0] = 0.0f;
	Values[2][1] = -s;
	Values[2][2] = c;
	Values[2][3] = 0.0f;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
	Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::RotationY( float angle )
{
	float c, s;
	SinCos( angle, s, c );

	Values[0][0] = c;
	Values[0][1] = 0.0f;
	Values[0][2] = -s;
	Values[0][3] = 0.0f;

	Values[1][0] = 0.0f;
	Values[1][1] = 1.0f;
	Values[1][2] = 0.0f;
	Values[1][3] = 0.0f;

	Values[2][0] = s;
	Values[2][1] = 0.0f;
	Values[2][2] = c;
	Values[2][3] = 0.0f;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
	Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::RotationZ( float angle )
{
	float c, s;
	SinCos( angle, s, c );

	Values[0][0] = c;
	Values[0][1] = s;
	Values[0][2] = 0.0f;
	Values[0][3] = 0.0f;

	Values[1][0] = -s;
	Values[1][1] = c;
	Values[1][2] = 0.0f;
	Values[1][3] = 0.0f;

	Values[2][0] = 0.0f;
	Values[2][1] = 0.0f;
	Values[2][2] = 1;
	Values[2][3] = 0.0f;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
	Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::Translation( float x, float y, float z )
{
	Indentity();
	Values[3][0] = x;
	Values[3][1] = y;
	Values[3][2] = z;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::RotationAxis( const Vector3D& axis, float angle )
{
	const float c = cosf( angle );
	const float s = sinf( angle );
	const float cc = 1.0f - c;

	Values[0][0] = cc * (axis.X * axis.X) + c;
	Values[0][1] = cc * (axis.X * axis.Y) + (axis.Z * s);
	Values[0][2] = cc * (axis.Z * axis.X) - (axis.Y * s);

	Values[1][0] = cc * (axis.X * axis.Y) - (axis.Z * s);
	Values[1][1] = cc * (axis.Y * axis.Y) + c;
	Values[1][2] = cc * (axis.Y * axis.Z) + (axis.X * s);

	Values[2][0] = cc * (axis.Z * axis.X) + (axis.Y * s);
	Values[2][1] = cc * (axis.Y * axis.Z) - (axis.X * s);
	Values[2][2] = cc * (axis.Z * axis.Z) + c;

	Values[3][0] = 0.0f;
	Values[3][1] = 0.0f;
	Values[3][2] = 0.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix44::Quaternion( float x, float y, float z, float w )
{
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	Values[0][0] = 1.0f - 2.0f * (yy + zz);
	Values[1][0] = 2.0f * (xy - wz);
	Values[2][0] = 2.0f * (xz + wy);
	Values[3][0] = 0.0f;

	Values[0][1] = 2.0f * (xy + wz);
	Values[1][1] = 1.0f - 2.0f * (xx + zz);
	Values[2][1] = 2.0f * (yz - wx);
	Values[3][1] = 0.0f;

	Values[0][2] = 2.0f * (xz - wy);
	Values[1][2] = 2.0f * (yz + wx);
	Values[2][2] = 1.0f - 2.0f * (xx + yy);
	Values[3][2] = 0.0f;

	Values[0][3] = 0.0f;
	Values[1][3] = 0.0f;
	Values[2][3] = 0.0f;
	Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::Mul( Matrix44& o, const Matrix44& in1, const Matrix44& in2 )
{
	Matrix44 _in1 = in1;
	Matrix44 _in2 = in2;

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			float v = 0.0f;
			for( int k = 0; k < 4; k++ )
			{
				v += _in1.Values[i][k] * _in2.Values[k][j];
			}
			o.Values[i][j] = v;
		}
	}
	return o;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix44& Matrix44::Inverse(  Matrix44& o, const Matrix44& in )
{
	float a11 = in.Values[0][0];
	float a12 = in.Values[0][1];
	float a13 = in.Values[0][2];
	float a14 = in.Values[0][3];
	float a21 = in.Values[1][0];
	float a22 = in.Values[1][1];
	float a23 = in.Values[1][2];
	float a24 = in.Values[1][3];
	float a31 = in.Values[2][0];
	float a32 = in.Values[2][1];
	float a33 = in.Values[2][2];
	float a34 = in.Values[2][3];
	float a41 = in.Values[3][0];
	float a42 = in.Values[3][1];
	float a43 = in.Values[3][2];
	float a44 = in.Values[3][3];

	/* 行列式の計算 */
	float b11 = + a22 * (a33 * a44 - a43 * a34) - a23 * (a32 * a44 - a42 * a34) + a24 * (a32 * a43 - a42 * a33);
	float b12 = - a12 * (a33 * a44 - a43 * a34) + a13 * (a32 * a44 - a42 * a34) - a14 * (a32 * a43 - a42 * a33);
	float b13 = + a12 * (a23 * a44 - a43 * a24) - a13 * (a22 * a44 - a42 * a24) + a14 * (a22 * a43 - a42 * a23);
	float b14 = - a12 * (a23 * a34 - a33 * a24) + a13 * (a22 * a34 - a32 * a24) - a14 * (a22 * a33 - a32 * a23);

	float b21 = - a21 * (a33 * a44 - a43 * a34) + a23 * (a31 * a44 - a41 * a34) - a24 * (a31 * a43 - a41 * a33);
	float b22 = + a11 * (a33 * a44 - a43 * a34) - a13 * (a31 * a44 - a41 * a34) + a14 * (a31 * a43 - a41 * a33);
	float b23 = - a11 * (a23 * a44 - a43 * a24) + a13 * (a21 * a44 - a41 * a24) - a14 * (a21 * a43 - a41 * a23);
	float b24 = + a11 * (a23 * a34 - a33 * a24) - a13 * (a21 * a34 - a31 * a24) + a14 * (a21 * a33 - a31 * a23);

	float b31 = + a21 * (a32 * a44 - a42 * a34) - a22 * (a31 * a44 - a41 * a34) + a24 * (a31 * a42 - a41 * a32);
	float b32 = - a11 * (a32 * a44 - a42 * a34) + a12 * (a31 * a44 - a41 * a34) - a14 * (a31 * a42 - a41 * a32);
	float b33 = + a11 * (a22 * a44 - a42 * a24) - a12 * (a21 * a44 - a41 * a24) + a14 * (a21 * a42 - a41 * a22);
	float b34 = - a11 * (a22 * a34 - a32 * a24) + a12 * (a21 * a34 - a31 * a24) - a14 * (a21 * a32 - a31 * a22);

	float b41 = - a21 * (a32 * a43 - a42 * a33) + a22 * (a31 * a43 - a41 * a33) - a23 * (a31 * a42 - a41 * a32);
	float b42 = + a11 * (a32 * a43 - a42 * a33) - a12 * (a31 * a43 - a41 * a33) + a13 * (a31 * a42 - a41 * a32);
	float b43 = - a11 * (a22 * a43 - a42 * a23) + a12 * (a21 * a43 - a41 * a23) - a13 * (a21 * a42 - a41 * a22);
	float b44 = + a11 * (a22 * a33 - a32 * a23) - a12 * (a21 * a33 - a31 * a23) + a13 * (a21 * a32 - a31 * a22);

	// 行列式の逆数をかける
	float Det = (a11 * b11) + (a12 * b21) + (a13 * b31) + (a14 * b41);
	if ( (-FLT_MIN <= Det) && (Det <= +FLT_MIN) )
	{
		o = in;
		return o;
	}

	float InvDet = 1.0f / Det;

	o.Values[0][0] = b11 * InvDet;
	o.Values[0][1] = b12 * InvDet;
	o.Values[0][2] = b13 * InvDet;
	o.Values[0][3] = b14 * InvDet;
	o.Values[1][0] = b21 * InvDet;
	o.Values[1][1] = b22 * InvDet;
	o.Values[1][2] = b23 * InvDet;
	o.Values[1][3] = b24 * InvDet;
	o.Values[2][0] = b31 * InvDet;
	o.Values[2][1] = b32 * InvDet;
	o.Values[2][2] = b33 * InvDet;
	o.Values[2][3] = b34 * InvDet;
	o.Values[3][0] = b41 * InvDet;
	o.Values[3][1] = b42 * InvDet;
	o.Values[3][2] = b43 * InvDet;
	o.Values[3][3] = b44 * InvDet;

	return o;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
#ifndef	__EFFEKSEER_INTERNAL_STRUCT_H__
#define	__EFFEKSEER_INTERNAL_STRUCT_H__

/**
	@file
	@brief	内部計算用構造体
	@note
	union等に使用するための構造体。<BR>

*/

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

template <typename T>
T ReadData( unsigned char*& pos )
{
	T result = *(T*)pos;
	pos += sizeof(T);
	return result;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_float
{
	float	max;
	float	min;

	void reset()
	{
		memset( this, 0 , sizeof(random_float) );
	};

	float getValue( const Manager& manager ) const
	{
		return (max - min) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_int
{
	int	max;
	int	min;

	void reset()
	{
		memset( this, 0 , sizeof(random_int) );
	};

	int getValue( const Manager& manager ) const
	{
		return (int) ( (max - min) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) ) + min;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct vector2d
{
	float	x;
	float	y;

	void reset()
	{
		assert( sizeof(vector2d) == sizeof(float) * 2 );
		memset( this, 0, sizeof(vector2d) );
	};

	void setValueToArg( Vector2D& v ) const
	{
		v.X = x;
		v.Y = y;
	}

	vector2d operator + ( const vector2d& o ) const
	{
		vector2d ret;
		ret.x = x + o.x;
		ret.y = y + o.y;
		return ret;
	}

	vector2d operator * (const float& o) const
	{
		vector2d ret;
		ret.x = x * o;
		ret.y = y * o;
		return ret;
	}

	vector2d& operator += ( const vector2d& o )
	{
		x += o.x;
		y += o.y;
		return *this;
	}

	vector2d& operator *= ( const float& o )
	{
		x *= o;
		y *= o;
		return *this;
	}
};

struct rectf
{
	float	x;
	float	y;
	float	w;
	float	h;
	void reset()
	{
		assert( sizeof(rectf) == sizeof(float) * 4 );
		memset( this, 0, sizeof(rectf) );
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_vector2d
{
	vector2d	max;
	vector2d	min;

	void reset()
	{
		memset( this, 0 , sizeof(random_vector2d) );
	};

	vector2d getValue( const Manager& manager ) const
	{
		vector2d r;
		r.x = (max.x - min.x) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min.x;
		r.y = (max.y - min.y) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min.y;
		return r;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct easing_float_without_random
{
	float easingA;
	float easingB;
	float easingC;

	void setValueToArg( float& o, const float start_, const float end_, float t ) const
	{
		float df = end_ - start_;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o = start_ + d * df;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct easing_float
{
	random_float	start;
	random_float	end;
	float easingA;
	float easingB;
	float easingC;

	void setValueToArg( float& o, const float start_, const float end_, float t ) const
	{
		float df = end_ - start_;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o = start_ + d * df;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct easing_vector2d
{
	random_vector2d	start;
	random_vector2d	end;
	float easingA;
	float easingB;
	float easingC;

	void setValueToArg( vector2d& o, const vector2d& start_, const vector2d& end_, float t ) const
	{
		float d_x = end_.x - start_.x;
		float d_y = end_.y - start_.y;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.x = start_.x + d * d_x;
		o.y = start_.y + d * d_y;
	}

	void setValueToArg( Vector2D& o, const vector2d& start_, const vector2d& end_, float t ) const
	{
		float d_x = end_.x - start_.x;
		float d_y = end_.y - start_.y;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.X = start_.x + d * d_x;
		o.Y = start_.y + d * d_y;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct vector3d
{
	float	x;
	float	y;
	float	z;

	void reset()
	{
		assert( sizeof(vector3d) == sizeof(float) * 3 );
		memset( this, 0, sizeof(vector3d) );
	};

	void setValueToArg( Vector3D& v ) const
	{
		v.X = x;
		v.Y = y;
		v.Z = z;
	}

	vector3d operator + ( const vector3d& o ) const
	{
		vector3d ret;
		ret.x = x + o.x;
		ret.y = y + o.y;
		ret.z = z + o.z;
		return ret;
	}

	vector3d operator - ( const vector3d& o ) const
	{
		vector3d ret;
		ret.x = x - o.x;
		ret.y = y - o.y;
		ret.z = z - o.z;
		return ret;
	}

	vector3d operator * ( const float& o ) const
	{
		vector3d ret;
		ret.x = x * o;
		ret.y = y * o;
		ret.z = z * o;
		return ret;
	}

	vector3d& operator += ( const vector3d& o )
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	vector3d& operator -= ( const vector3d& o )
	{
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	vector3d& operator *= ( const float& o )
	{
		x *= o;
		y *= o;
		z *= o;
		return *this;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_vector3d
{
	vector3d	max;
	vector3d	min;

	void reset()
	{
		memset( this, 0 , sizeof(random_vector3d) );
	};

	vector3d getValue( const Manager& manager ) const
	{
		vector3d r;
		r.x = (max.x - min.x) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min.x;
		r.y = (max.y - min.y) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min.y;
		r.z = (max.z - min.z) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + min.z;
		return r;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct easing_vector3d
{
	random_vector3d	start;
	random_vector3d	end;
	float easingA;
	float easingB;
	float easingC;

	void setValueToArg( vector3d& o, const vector3d& start_, const vector3d& end_, float t ) const
	{
		float d_x = end_.x - start_.x;
		float d_y = end_.y - start_.y;
		float d_z = end_.z - start_.z;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.x = start_.x + d * d_x;
		o.y = start_.y + d * d_y;
		o.z = start_.z + d * d_z;
	}

	void setValueToArg( Vector3D& o, const vector3d& start_, const vector3d& end_, float t ) const
	{
		float d_x = end_.x - start_.x;
		float d_y = end_.y - start_.y;
		float d_z = end_.z - start_.z;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.X = start_.x + d * d_x;
		o.Y = start_.y + d * d_y;
		o.Z = start_.z + d * d_z;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct color
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;
	
	void reset()
	{
		assert( sizeof(color) == 4 );
		memset( this, 255, sizeof(color) );
	}

	void setValueToArg( Color& c ) const
	{
		c.R = r;
		c.G = g;
		c.B = b;
		c.A = a;
	}

	static color mul( const color& in1, const color& in2 )
	{
		color o;
		o.r = (uint8_t)((float)in1.r * (float)in2.r / 255.0f);
		o.g = (uint8_t)((float)in1.g * (float)in2.g / 255.0f);
		o.b = (uint8_t)((float)in1.b * (float)in2.b / 255.0f);
		o.a = (uint8_t)((float)in1.a * (float)in2.a / 255.0f);
		return o;
	}

	static color mul( const color& in1, float in2 )
	{
		color o;
		o.r = (uint8_t)((float)in1.r * in2);
		o.g = (uint8_t)((float)in1.g * in2);
		o.b = (uint8_t)((float)in1.b * in2);
		o.a = (uint8_t)((float)in1.a * in2);
		return o;
	}
};

inline color HSVToRGB(color hsv) {
	int H = hsv.r, S = hsv.g, V = hsv.b;
	int Hi, R=0, G=0, B=0, p, q, t;
	float f, s;

	if (H >= 252) H = 252;
	Hi = (H / 42);
	f = H / 42.0f - Hi;
	Hi = Hi % 6;
	s = S / 255.0f;
	p = (int)((V * (1 - s)));
	q = (int)((V * (1 - f * s)));
	t = (int)((V * (1 - (1 - f) * s)));

	switch (Hi) {
	case 0: R = V; G = t; B = p; break;
	case 1: R = q; G = V; B = p; break;
	case 2: R = p; G = V; B = t; break;
	case 3: R = p; G = q; B = V; break;
	case 4: R = t; G = p; B = V; break;
	case 5: R = V; G = p; B = q; break;
	}
	color result;
	result.r = R;
	result.g = G;
	result.b = B;
	result.a = hsv.a;
	return result;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_color
{
	ColorMode mode;
	color	max;
	color	min;

	void reset()
	{
		assert( sizeof(random_color) == 12 );
		mode = COLOR_MODE_RGBA;
		max.reset();
		min.reset();
	};

	color getValue( const Manager& manager ) const
	{
		color r = getDirectValue( manager );
		if( mode == COLOR_MODE_HSVA )
		{
			r = HSVToRGB( r );
		}
		return r;
	}
	
	color getDirectValue( const Manager& manager ) const
	{
		color r;
		r.r = (uint8_t)(((float)max.r - (float)min.r) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + (float)min.r);
		r.g = (uint8_t)(((float)max.g - (float)min.g) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + (float)min.g);
		r.b = (uint8_t)(((float)max.b - (float)min.b) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + (float)min.b);
		r.a = (uint8_t)(((float)max.a - (float)min.a) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + (float)min.a);
		return r;
	}

	void load( int version, unsigned char*& pos )
	{
		if( version >= 4 )
		{
			mode = (ColorMode)ReadData<uint8_t>( pos );
			pos++;	// reserved
		}
		else
		{
			mode = COLOR_MODE_RGBA;
		}
		max = ReadData<color>( pos );
		min = ReadData<color>( pos );
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct easing_color
{
	random_color	start;
	random_color	end;
	float easingA;
	float easingB;
	float easingC;

	void setValueToArg( color& o, const color& start_, const color& end_, float t ) const
	{
		assert( start.mode == end.mode );
		int d_r = end_.r - start_.r;
		int d_g = end_.g - start_.g;
		int d_b = end_.b - start_.b;
		int d_a = end_.a - start_.a;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.r = (uint8_t)Clamp( start_.r + d * d_r, 255.0f, 0.0f );
		o.g = (uint8_t)Clamp( start_.g + d * d_g, 255.0f, 0.0f );
		o.b = (uint8_t)Clamp( start_.b + d * d_b, 255.0f, 0.0f );
		o.a = (uint8_t)Clamp( start_.a + d * d_a, 255.0f, 0.0f );
		if( start.mode == COLOR_MODE_HSVA )
		{
			o = HSVToRGB( o );
		}
	}

	void setValueToArg( Color& o, const color& start_, const color& end_, float t ) const
	{
		assert( start.mode == end.mode );
		int d_r = end_.r - start_.r;
		int d_g = end_.g - start_.g;
		int d_b = end_.b - start_.b;
		int d_a = end_.a - start_.a;
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o.R = (uint8_t)Clamp( start_.r + d * d_r, 255.0f, 0.0f );
		o.G = (uint8_t)Clamp( start_.g + d * d_g, 255.0f, 0.0f );
		o.B = (uint8_t)Clamp( start_.b + d * d_b, 255.0f, 0.0f );
		o.A = (uint8_t)Clamp( start_.a + d * d_a, 255.0f, 0.0f );
		if( start.mode == COLOR_MODE_HSVA )
		{
			*(color*)&o = HSVToRGB( *(color*)&o );
		}
	}

	color getStartValue( const Manager& manager ) const
	{
		return start.getDirectValue( manager );
	}
	
	color getEndValue( const Manager& manager ) const
	{
		return end.getDirectValue( manager );
	}

	void load( int version, unsigned char*& pos )
	{
		start.load( version, pos );
		end.load( version, pos );
		easingA = ReadData<float>( pos );
		easingB = ReadData<float>( pos );
		easingC = ReadData<float>( pos );
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INTERNAL_STRUCT_H__﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
	
#ifdef _WIN32
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
CriticalSection::CriticalSection()
{
	::InitializeCriticalSection( &m_criticalSection );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection( &m_criticalSection );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
void CriticalSection::Enter() const
{
	::EnterCriticalSection( &m_criticalSection );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
void CriticalSection::Leave() const
{
	::LeaveCriticalSection( &m_criticalSection );
}
#else
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
CriticalSection::CriticalSection()
{
	pthread_mutex_init( &m_mutex, NULL );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
CriticalSection::~CriticalSection()
{
	pthread_mutex_destroy( &m_mutex );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
void CriticalSection::Enter() const
{
	pthread_mutex_lock( &m_mutex );
}

//-----------------------------------------------------------------------------------
/**
*/
//-----------------------------------------------------------------------------------
void CriticalSection::Leave() const
{
	pthread_mutex_unlock( &m_mutex );
}
#endif
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

﻿
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 

#ifdef _WIN32

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
/* DWORDを置きかえ */
unsigned long EFK_STDCALL Thread::ThreadProc(void* arguments)
{
	Thread* thread = (Thread*)(arguments);

	thread->m_mainProc( thread->m_data );

	thread->m_cs.Enter();

	thread->m_mainProc	= NULL;
	thread->m_data		= NULL;

	::CloseHandle( thread->m_thread );
	thread->m_thread		= NULL;

	thread->m_cs.Leave();

	return 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Thread::Thread()
	: m_data		( NULL )
	, m_thread		( NULL )
	, m_mainProc	( NULL )
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Thread::~Thread()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::Create( void (*threadFunc)( void* ), void* data )
{
	m_cs.Enter();

	if ( m_thread == NULL )
	{
		m_data		= data;
		m_mainProc	= threadFunc;
		m_thread	= ::CreateThread( NULL, 0, ThreadProc, this, CREATE_SUSPENDED, NULL );

		// スレッド開始
		::SetThreadPriority( m_thread, THREAD_PRIORITY_NORMAL );
		::ResumeThread( m_thread );

		m_cs.Leave();
		return true;
	}

	m_cs.Leave();
	return false;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::IsExitThread() const
{
	m_cs.Enter();
	bool ret = m_thread == NULL;
	m_cs.Leave();
	return ret;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::Wait() const
{
	::WaitForSingleObject( m_thread, INFINITE );

	return true;
}

#else

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void* Thread::ThreadProc( void* arguments )
{
	Thread* thread = (Thread*)(arguments);

	thread->m_mainProc( thread->m_data );

	thread->m_cs.Enter();

	thread->m_mainProc	= NULL;
	thread->m_data		= NULL;

	pthread_detach( thread->m_thread );

	thread->m_running	= false;

	thread->m_cs.Leave();

	return 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Thread::Thread()
	: m_running		( false )
	, m_thread		()
	, m_data		( NULL )
	, m_mainProc	( NULL )
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Thread::~Thread()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::Create( void (*threadFunc)( void* ), void* data )
{
	m_cs.Enter();

	if ( !m_running )
	{
		m_data		= data;
		m_mainProc	= threadFunc;

		pthread_attr_t attr;
		pthread_attr_init( &attr );

		// スレッド開始
		m_running = true;
		pthread_create( &m_thread, &attr, ThreadProc, this );
		
		m_cs.Leave();
		return true;
	}

	m_cs.Leave();
	return false;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::IsExitThread() const
{
	m_cs.Enter();
	bool ret = !m_running;
	m_cs.Leave();
	return ret;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Thread::Wait() const
{
	pthread_join( m_thread, NULL );

	return true;
}
#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
﻿
#ifndef	__EFFEKSEER_DEFAULTEFFECTLOADER_H__
#define	__EFFEKSEER_DEFAULTEFFECTLOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	標準のエフェクトファイル読み込み破棄関数指定クラス
*/
class DefaultEffectLoader
	: public EffectLoader
{
	DefaultFileInterface m_defaultFileInterface;
	FileInterface* m_fileInterface;
public:

	DefaultEffectLoader( FileInterface* fileInterface = NULL );

	virtual ~DefaultEffectLoader();

	bool Load( const EFK_CHAR* path, void*& data, int32_t& size );

	void Unload( void* data, int32_t size );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_DEFAULTEFFECTLOADER_H__
﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultEffectLoader::DefaultEffectLoader( FileInterface* fileInterface )
	: m_fileInterface( fileInterface )
{
	if( m_fileInterface == NULL )
	{
		m_fileInterface = &m_defaultFileInterface;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultEffectLoader::~DefaultEffectLoader()
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool DefaultEffectLoader::Load( const EFK_CHAR* path, void*& data, int32_t& size )
{
	assert( path != NULL );

	data = NULL;
	size = 0;

	std::unique_ptr<FileReader> 
		reader( m_fileInterface->OpenRead( path ) );
	if( reader.get() == NULL ) return false;

	size = reader->GetLength();
	data = new uint8_t[size];
	reader->Read( data, size );

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DefaultEffectLoader::Unload( void* data, int32_t size )
{
	uint8_t* data8 = (uint8_t*)data;
	ES_SAFE_DELETE_ARRAY( data8 );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿
#ifndef	__EFFEKSEER_DEFAULT_FILE_H__
#define	__EFFEKSEER_DEFAULT_FILE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	標準のファイル読み込みクラス
*/

class DefaultFileReader : public FileReader
{
private:
	FILE* m_filePtr;

public:
	DefaultFileReader( FILE* filePtr );

	~DefaultFileReader();

	size_t Read( void* buffer, size_t size );

	void Seek( int position );

	int GetPosition();

	size_t GetLength();
};

class DefaultFileWriter : public FileWriter
{
private:
	FILE* m_filePtr;

public:
	DefaultFileWriter( FILE* filePtr );

	~DefaultFileWriter();

	size_t Write( const void* buffer, size_t size );

	void Flush();

	void Seek( int position );

	int GetPosition();

	size_t GetLength();
};

class DefaultFileInterface : public FileInterface
{
private:

public:
	FileReader* OpenRead( const EFK_CHAR* path );

	FileWriter* OpenWrite( const EFK_CHAR* path );
};


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_DEFAULT_FILE_H__
﻿
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultFileReader::DefaultFileReader( FILE* filePtr )
	: m_filePtr( filePtr )
{
	assert( filePtr != NULL );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultFileReader::~DefaultFileReader()
{
	fclose( m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
size_t DefaultFileReader::Read( void* buffer, size_t size )
{
	return fread( buffer, 1, size, m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DefaultFileReader::Seek( int position )
{
	fseek( m_filePtr, (size_t)position, SEEK_SET );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int DefaultFileReader::GetPosition()
{
	return (int)ftell( m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
size_t DefaultFileReader::GetLength()
{
	long position = ftell( m_filePtr );
	fseek( m_filePtr, 0, SEEK_END );
	long length = ftell( m_filePtr );
	fseek( m_filePtr, position, SEEK_SET );
	return (size_t)length;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultFileWriter::DefaultFileWriter( FILE* filePtr )
	: m_filePtr( filePtr )
{
	assert( filePtr != NULL );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DefaultFileWriter::~DefaultFileWriter()
{
	fclose( m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
size_t DefaultFileWriter::Write( const void* buffer, size_t size )
{
	return fwrite( buffer, 1, size, m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DefaultFileWriter::Flush()
{
	fflush( m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DefaultFileWriter::Seek( int position )
{
	fseek( m_filePtr, (size_t)position, SEEK_SET );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int DefaultFileWriter::GetPosition()
{
	return (int)ftell( m_filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
size_t DefaultFileWriter::GetLength()
{
	long position = ftell( m_filePtr );
	fseek( m_filePtr, 0, SEEK_END );
	long length = ftell( m_filePtr );
	fseek( m_filePtr, position, SEEK_SET );
	return (size_t)length;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FileReader* DefaultFileInterface::OpenRead( const EFK_CHAR* path )
{
	FILE* filePtr = NULL;
#ifdef _WIN32
	_wfopen_s( &filePtr, (const wchar_t*)path, L"rb" );
#else
	int8_t path8[256];
	ConvertUtf16ToUtf8( path8, 256, (const int16_t*)path );
	filePtr = fopen( (const char*)path8, "rb" );
#endif
	
	if( filePtr == NULL )
	{
		return NULL;
	}

	return new DefaultFileReader( filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FileWriter* DefaultFileInterface::OpenWrite( const EFK_CHAR* path )
{
	FILE* filePtr = NULL;
#ifdef _WIN32
	_wfopen_s( &filePtr, (const wchar_t*)path, L"wb" );
#else
	int8_t path8[256];
	ConvertUtf16ToUtf8( path8, 256, (const int16_t*)path );
	filePtr = fopen( (const char*)path8, "wb" );
#endif
	
	if( filePtr == NULL )
	{
		return NULL;
	}

	return new DefaultFileWriter( filePtr );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
#ifndef	__EFFEKSEER_FCURVES_H__
#define	__EFFEKSEER_FCURVES_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	
class FCurve
{
private:
	enum eFCurveEdge
	{
		FC_EDGE_Constant = 0,
		FC_EDGE_Loop = 1,
		FC_EDGE_LoopInversely = 2,
		FC_EDGE_DWORD = 0x7fffffff,
	};

private:
	int32_t				m_offset;
	int32_t				m_len;
	int32_t				m_freq;
	eFCurveEdge			m_start;
	eFCurveEdge			m_end;
	std::vector<float>	m_keys;

	float				m_defaultValue;
	float				m_offsetMax;
	float				m_offsetMin;
public:
	FCurve( float defaultValue );
	int32_t Load( void* data, int32_t version );

	float GetValue( int32_t frame );

	float GetOffset( const Manager& manager ) const;

	void SetDefaultValue( float value ) { m_defaultValue = value; }

	void ChangeCoordinate();

	void Maginify(float value );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class FCurveVector3D
{
public:
	FCurve X;
	FCurve Y;
	FCurve Z;

	FCurveVector3D();
	int32_t Load( void* data, int32_t version );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class FCurveVectorColor
{
public:
	FCurve R;
	FCurve G;
	FCurve B;
	FCurve A;

	FCurveVectorColor();
	int32_t Load( void* data, int32_t version );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_FCURVES_H__
﻿
#ifndef	__EFFEKSEER_EFFECTNODE_H__
#define	__EFFEKSEER_EFFECTNODE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum BindType
{
	BindType_NotBind = 0,
	BindType_NotBind_Root = 3,
	BindType_WhenCreating = 1,
	BindType_Always = 2,

	BindType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class StandardColorParameter
{
public:
	enum
	{
		Fixed = 0,
		Random = 1,
		Easing = 2,
		FCurve_RGBA = 3,
		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
			color all;
		} fixed;

		struct
		{
			random_color all;
		} random;

		struct
		{
			easing_color all;
		} easing;

		struct
		{
			FCurveVectorColor* FCurve;
		} fcurve_rgba;
	};

	StandardColorParameter()
	{
		type = Fixed;
	}

	~StandardColorParameter()
	{
		if( type == FCurve_RGBA )
		{
			ES_SAFE_DELETE( fcurve_rgba.FCurve );
		}
	}

	void load( uint8_t*& pos, int32_t version )
	{
		memcpy( &type, pos, sizeof(int) );
		pos += sizeof(int);
		
		if( type == Fixed )
		{
			memcpy( &fixed, pos, sizeof(fixed) );
			pos += sizeof(fixed);
		}
		else if( type == Random )
		{
			random.all.load( version, pos );
		}
		else if( type == Easing )
		{
			easing.all.load( version, pos );
		}
		else if( type == FCurve_RGBA )
		{
			fcurve_rgba.FCurve = new FCurveVectorColor();
			int32_t size = fcurve_rgba.FCurve->Load(  pos, version );
			pos += size;
		}
	}
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterCommonValues_8
{
	int MaxGeneration;
	BindType TranslationBindType;
	BindType RotationBindType;
	BindType ScalingBindType;
	int		RemoveWhenLifeIsExtinct;
	int		RemoveWhenParentIsRemoved;
	int		RemoveWhenChildrenIsExtinct;
	random_int	life;
	float GenerationTime;
	float GenerationTimeOffset;
};

struct ParameterCommonValues
{
	int MaxGeneration;
	BindType TranslationBindType;
	BindType RotationBindType;
	BindType ScalingBindType;
	int		RemoveWhenLifeIsExtinct;
	int		RemoveWhenParentIsRemoved;
	int		RemoveWhenChildrenIsExtinct;
	random_int	life;
	random_float GenerationTime;
	random_float GenerationTimeOffset;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ParameterTranslationType
{
	ParameterTranslationType_Fixed = 0,
	ParameterTranslationType_PVA = 1,
	ParameterTranslationType_Easing = 2,
	ParameterTranslationType_FCurve = 3,

	ParameterTranslationType_None = 0x7fffffff - 1,

	ParameterTranslationType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterTranslationFixed
{
	Vector3D Position;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterTranslationPVA
{
	random_vector3d	location;
	random_vector3d	velocity;
	random_vector3d	acceleration;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct LocationAbsParameter
{
	enum
	{
		None = 0,
		Gravity = 1,
		AttractiveForce = 2,
		
		//UniformlyAttractiveForce = 3,
		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
		
		} none;

		vector3d	gravity;

		struct {
			float	force;
			float	control;
			float	minRange;
			float	maxRange;
		} attractiveForce;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ParameterRotationType
{
	ParameterRotationType_Fixed = 0,
	ParameterRotationType_PVA = 1,
	ParameterRotationType_Easing = 2,
	ParameterRotationType_AxisPVA = 3,
	ParameterRotationType_AxisEasing = 4,

	ParameterRotationType_FCurve = 5,

	ParameterRotationType_None = 0x7fffffff - 1,

	ParameterRotationType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterRotationFixed
{
	Vector3D Position;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterRotationPVA
{
	random_vector3d	rotation;
	random_vector3d	velocity;
	random_vector3d	acceleration;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterRotationAxisPVA
{
	random_vector3d	axis;
	random_float	rotation;
	random_float	velocity;
	random_float	acceleration;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterRotationAxisEasing
{
	random_vector3d	axis;
	easing_float	easing;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ParameterScalingType
{
	ParameterScalingType_Fixed = 0,
	ParameterScalingType_PVA = 1,
	ParameterScalingType_Easing = 2,
	ParameterScalingType_SinglePVA = 3,
	ParameterScalingType_SingleEasing = 4,
	ParameterScalingType_FCurve = 5,
	
	ParameterScalingType_None = 0x7fffffff - 1,

	ParameterScalingType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterScalingFixed
{
	Vector3D Position;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterScalingPVA
{
	random_vector3d Position;
	random_vector3d Velocity;
	random_vector3d Acceleration;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterScalingSinglePVA
{
	random_float Position;
	random_float Velocity;
	random_float Acceleration;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterGenerationLocation
{
	int	EffectsRotation;

	enum
	{
		TYPE_POINT = 0,
		TYPE_SPHERE = 1,
		TYPE_MODEL = 2,
		TYPE_CIRCLE = 3,

		TYPE_DWORD = 0x7fffffff,
	} type;

	enum eModelType
	{
		MODELTYPE_RANDOM = 0,
		MODELTYPE_VERTEX = 1,
		MODELTYPE_VERTEX_RANDOM = 2,
		MODELTYPE_FACE = 3,
		MODELTYPE_FACE_RANDOM = 4,

		MODELTYPE_DWORD = 0x7fffffff,
	};

	enum eCircleType
	{
		CIRCLE_TYPE_RANDOM = 0,
		CIRCLE_TYPE_ORDER = 1,
		CIRCLE_TYPE_REVERSE_ORDER = 2,
	};

	union
	{
		struct
		{
			random_vector3d location;
		} point;

		struct
		{
			random_float radius;
			random_float rotation_x;
			random_float rotation_y;
		} sphere;

		struct
		{
			int32_t		index;
			eModelType	type;
		} model;

		struct
		{
			int32_t			division;
			random_float	radius;
			random_float	angle_start;
			random_float	angle_end;
			eCircleType		type;
		} circle;
	};

	void load( uint8_t*& pos )
	{
		memcpy( &EffectsRotation, pos, sizeof(int) );
		pos += sizeof(int);

		memcpy( &type, pos, sizeof(int) );
		pos += sizeof(int);

		if( type == TYPE_POINT )
		{
			memcpy( &point, pos, sizeof(point) );
			pos += sizeof(point);
		}
		else if( type == TYPE_SPHERE )
		{
			memcpy( &sphere, pos, sizeof(sphere) );
			pos += sizeof(sphere);
		}
		else if( type == TYPE_MODEL )
		{
			memcpy( &model, pos, sizeof(model) );
			pos += sizeof(model);
		}
		else if( type == TYPE_CIRCLE )
		{
			memcpy( &circle, pos, sizeof(circle) );
			pos += sizeof(circle);
		}
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterTexture
{
	int32_t				ColorTextureIndex;
	AlphaBlendType	AlphaBlend;

	TextureFilterType	FilterType;

	TextureWrapType	WrapType;

	bool				ZWrite;

	bool				ZTest;

	bool				Distortion;

	float				DistortionIntensity;

	enum
	{
		FADEIN_ON = 1,
		FADEIN_OFF = 0,

		FADEIN_DWORD = 0x7fffffff,
	} FadeInType;

	struct
	{
		float	Frame;
		easing_float_without_random	Value;
	} FadeIn;

	enum
	{
		FADEOUT_ON = 1,
		FADEOUT_OFF = 0,

		FADEOUT_DWORD = 0x7fffffff,
	} FadeOutType;

	struct
	{
		float	Frame;
		easing_float_without_random	Value;
	} FadeOut;

	enum
	{
		UV_DEFAULT = 0,
		UV_FIXED = 1,
		UV_ANIMATION = 2,
		UV_SCROLL = 3,

		UV_DWORD = 0x7fffffff,
	} UVType;

	union
	{
		struct
		{
		} Default;

		struct
		{
			rectf	Position;
		} Fixed;

		struct
		{
			rectf	Position;
			int32_t	FrameLength;
			int32_t	FrameCountX;
			int32_t	FrameCountY;

			enum
			{
				LOOPTYPE_ONCE = 0,
				LOOPTYPE_LOOP = 1,
				LOOPTYPE_REVERSELOOP = 2,
				
				LOOPTYPE_DWORD = 0x7fffffff,
			} LoopType;

		} Animation;

		struct
		{
			rectf		Position;
			vector2d	Speed;
		} Scroll;

	} UV;

	void reset()
	{
		memset( this, 0, sizeof(ParameterTexture) );
	}

	void load( uint8_t*& pos, int32_t version )
	{
		memset( this, 0, sizeof(ParameterTexture) );

		memcpy( &ColorTextureIndex, pos, sizeof(int) );
		pos += sizeof(int);

		memcpy( &AlphaBlend, pos, sizeof(int) );
		pos += sizeof(int);

		memcpy( &FilterType, pos, sizeof(int) );
		pos += sizeof(int);

		memcpy( &WrapType, pos, sizeof(int) );
		pos += sizeof(int);

		if( version >= 5 )
		{
			int32_t zwrite, ztest = 0;

			memcpy( &ztest, pos, sizeof(int32_t) );
			pos += sizeof(int32_t);

			memcpy( &zwrite, pos, sizeof(int32_t) );
			pos += sizeof(int32_t);

			ZWrite = zwrite != 0;
			ZTest = ztest != 0;
		}
		else
		{
			ZWrite = false;
			ZTest = true;
		}

		memcpy( &FadeInType, pos, sizeof(int) );
		pos += sizeof(int);
		
		if( FadeInType == FADEIN_ON )
		{
			memcpy( &FadeIn, pos, sizeof(FadeIn) );
			pos += sizeof(FadeIn);
		}

		memcpy( &FadeOutType, pos, sizeof(int) );
		pos += sizeof(int);
		
		if( FadeOutType == FADEOUT_ON )
		{
			memcpy( &FadeOut, pos, sizeof(FadeOut) );
			pos += sizeof(FadeOut);
		}

		memcpy( &UVType, pos, sizeof(int) );
		pos += sizeof(int);

		if( UVType == UV_DEFAULT )
		{
		}
		else if( UVType == UV_FIXED )
		{
			memcpy( &UV.Fixed, pos, sizeof(UV.Fixed) );
			pos += sizeof(UV.Fixed);
		}
		else if( UVType == UV_ANIMATION )
		{
			memcpy( &UV.Animation, pos, sizeof(UV.Animation) );
			pos += sizeof(UV.Animation);
		}
		else if( UVType == UV_SCROLL )
		{
			memcpy( &UV.Scroll, pos, sizeof(UV.Scroll) );
			pos += sizeof(UV.Scroll);
		}

		if (version >= 9)
		{
			int32_t distortion = 0;

			memcpy(&distortion, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			Distortion = distortion > 0;

			memcpy(&DistortionIntensity, pos, sizeof(float));
			pos += sizeof(float);
			
		}
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ParameterSoundType
{
	ParameterSoundType_None = 0,
	ParameterSoundType_Use = 1,

	ParameterSoundType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ParameterSoundPanType
{
	ParameterSoundPanType_2D = 0,
	ParameterSoundPanType_3D = 1,

	ParameterSoundPanType_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ParameterSound
{
	int32_t			WaveId;
	random_float	Volume;
	random_float	Pitch;
	ParameterSoundPanType PanType;
	random_float	Pan;
	float			Distance;
	random_int		Delay;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum eRenderingOrder
{
	RenderingOrder_FirstCreatedInstanceIsFirst = 0,
	RenderingOrder_FirstCreatedInstanceIsLast = 1,

	RenderingOrder_DWORD = 0x7fffffff,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	ノードインスタンス生成クラス
	@note
	エフェクトのノードの実体を生成する。
*/
class EffectNodeImplemented
	: public EffectNode
{
	friend class Manager;
	friend class EffectImplemented;
	friend class Instance;

protected:
	// 所属しているパラメーター
	Effect*	m_effect;

	// 子ノード
	std::vector<EffectNodeImplemented*>	m_Nodes;

	// ユーザーデータ
	void* m_userData;

	// コンストラクタ
	EffectNodeImplemented(Effect* effect, unsigned char*& pos);

	// デストラクタ
	virtual ~EffectNodeImplemented();

	// 読込
	void LoadParameter( unsigned char*& pos, EffectNode* parent, Setting* setting );

	// 初期化
	void Initialize();

public:

	/**
		@brief	描画するか?
	*/
	bool IsRendered;

	ParameterCommonValues		CommonValues;

	ParameterTranslationType	TranslationType;
	ParameterTranslationFixed	TranslationFixed;
	ParameterTranslationPVA		TranslationPVA;
	easing_vector3d				TranslationEasing;
	FCurveVector3D*				TranslationFCurve;

	LocationAbsParameter		LocationAbs;

	ParameterRotationType		RotationType;
	ParameterRotationFixed		RotationFixed;
	ParameterRotationPVA		RotationPVA;
	easing_vector3d				RotationEasing;
	FCurveVector3D*				RotationFCurve;

	ParameterRotationAxisPVA	RotationAxisPVA;
	ParameterRotationAxisEasing	RotationAxisEasing;

	ParameterScalingType		ScalingType;
	ParameterScalingFixed		ScalingFixed;
	ParameterScalingPVA			ScalingPVA;
	easing_vector3d				ScalingEasing;
	ParameterScalingSinglePVA	ScalingSinglePVA;
	easing_float				ScalingSingleEasing;
	FCurveVector3D*				ScalingFCurve;

	ParameterGenerationLocation	GenerationLocation;

	ParameterTexture			Texture;

	ParameterSoundType			SoundType;
	ParameterSound				Sound;

	eRenderingOrder				RenderingOrder;

	/**
		@biref	オプション読み込み
	*/
	void LoadOption( uint8_t*& pos );

	Effect* GetEffect() const override;

	int GetChildrenCount() const override;

	EffectNode* GetChild( int index ) const override;

	EffectBasicRenderParameter GetBasicRenderParameter() override;

	void SetBasicRenderParameter(EffectBasicRenderParameter param) override;

	/**
		@brief	描画部分の読込
	*/
	virtual void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	/**
		@brief	描画開始
	*/
	virtual void BeginRendering(int32_t count, Manager* manager);

	/**
		@brief	グループ描画開始
	*/
	virtual void BeginRenderingGroup(InstanceGroup* group, Manager* manager);

	/**
		@brief	描画
	*/
	virtual void Rendering(const Instance& instance, Manager* manager);

	/**
		@brief	描画終了
	*/
	virtual void EndRendering(Manager* manager);

	/**
		@brief	インスタンスグループ描画時初期化
	*/
	virtual void InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager);

	/**
		@brief	描画部分初期化
	*/
	virtual void InitializeRenderedInstance( Instance& instance, Manager* manager );

	/**
		@brief	描画部分更新
	*/
	virtual void UpdateRenderedInstance(Instance& instance, Manager* manager);

	/**
		@brief	描画部分更新
	*/
	virtual float GetFadeAlpha( const Instance& instance );

	/**
		@brief	サウンド再生
	*/
	virtual void PlaySound_(Instance& instance, SoundTag tag, Manager* manager);

	/**
		@brief	エフェクトノード生成
	*/
	static EffectNodeImplemented* Create(Effect* effect, EffectNode* parent, unsigned char*& pos);

	/**
		@brief	ノードの種類取得
	*/
	virtual eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_NONE; }
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_EFFECTNODE_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_MODEL_H__
#define	__EFFEKSEER_ParameterNODE_MODEL_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class EffectNodeModel
	: public EffectNodeImplemented
{
	friend class Manager;
	friend class Effect;
	friend class Instance;

public:

	struct InstanceValues
	{
		// 色
		color _color;

		union 
		{
			struct
			{
			
			} fixed;

			struct
			{

			} random;

			struct
			{
				color start;
				color  end;

			} easing;

			struct
			{
				float	offset[4];
			} fcurve_rgba;

		} allColorValues;
	};

public:
	AlphaBlendType		AlphaBlend;
	int32_t			ModelIndex;
	int32_t			NormalTextureIndex;
	bool			Lighting;
	CullingType	Culling;

	StandardColorParameter	AllColor;

	EffectNodeModel( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
	{
	}

	~EffectNodeModel()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	void BeginRendering(int32_t count, Manager* manager);

	void Rendering(const Instance& instance, Manager* manager);

	void EndRendering(Manager* manager);

	void InitializeRenderedInstance(Instance& instance, Manager* manager);

	void UpdateRenderedInstance(Instance& instance, Manager* manager);

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_MODEL; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_MODEL_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_RIBBON_H__
#define	__EFFEKSEER_ParameterNODE_RIBBON_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RibbonAllColorParameter
{
	enum
	{
		Fixed = 0,
		Random = 1,
		Easing = 2,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
			color all;
		} fixed;

		struct
		{
			random_color all;
		} random;

		struct
		{
			easing_color all;
		} easing;
	};
};

struct RibbonColorParameter
{
	enum
	{
		Default = 0,
		Fixed = 1,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
		
		} def;

		struct
		{
			color l;
			color r;
		} fixed;
	};
};

struct RibbonPositionParameter
{
	enum
	{
		Default = 0,
		Fixed = 1,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
		
		} def;

		struct
		{
			float l;
			float r;
		} fixed;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class EffectNodeRibbon
	: public EffectNodeImplemented
{
public:

	struct InstanceValues
	{
		// 色
		color _color;

		union 
		{
			struct
			{
			
			} fixed;

			struct
			{

			} random;

			struct
			{
				color start;
				color  end;

			} easing;

		} allColorValues;

		union
		{
	
		} colorValues;

		union
		{
	
		} positionValues;
	};

	RibbonRenderer::NodeParameter	m_nodeParameter;
	RibbonRenderer::InstanceParameter m_instanceParameter;
public:

	AlphaBlendType		AlphaBlend;

	int				ViewpointDependent;

	RibbonAllColorParameter	RibbonAllColor;

	RibbonColorParameter RibbonColor;
	RibbonPositionParameter RibbonPosition;

	int RibbonTexture;

	EffectNodeRibbon( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
	{
	}

	~EffectNodeRibbon()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	void BeginRendering(int32_t count, Manager* manager);

	void BeginRenderingGroup(InstanceGroup* group, Manager* manager);

	void Rendering(const Instance& instance, Manager* manager);

	void EndRendering(Manager* manager);

	void InitializeRenderedInstance(Instance& instance, Manager* manager);

	void UpdateRenderedInstance(Instance& instance, Manager* manager);

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_RIBBON; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_RIBBON_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_RING_H__
#define	__EFFEKSEER_ParameterNODE_RING_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingSingleParameter
{
	enum
	{
		Fixed = 0,
		Random = 1,
		Easing = 2,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		float fixed;
		random_float random;
		easing_float easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingLocationParameter
{
	enum
	{
		Fixed = 0,
		PVA = 1,
		Easing = 2,

		Parameter_DWORD = 0x7fffffff,
	} type;
	
	union
	{
		struct
		{
			vector2d location;
		} fixed;
	
		struct
		{
			random_vector2d	location;
			random_vector2d	velocity;
			random_vector2d	acceleration;
		} pva;
		
		easing_vector2d easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingColorParameter
{
	enum
	{
		Fixed = 0,
		Random = 1,
		Easing = 2,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{	
		color fixed;
		random_color random;
		easing_color easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingSingleValues
{
	float	current;
	union
	{
		struct
		{
			
		} fixed;

		struct
		{

		} random;

		struct
		{
			float  start;
			float  end;
		} easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingLocationValues
{
	vector2d	current;
	union
	{
		struct
		{
	
		} fixed;

		struct
		{
			vector2d  start;
			vector2d  velocity;
			vector2d  acceleration;
		} pva;

		struct
		{
			vector2d  start;
			vector2d  end;
		} easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RingColorValues
{
	color	current;
	union
	{
		struct
		{
	
		} fixed;

		struct
		{

		} random;

		struct
		{
			color  start;
			color  end;
		} easing;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class EffectNodeRing
	: public EffectNodeImplemented
{
	friend class Manager;
	friend class Effect;
	friend class Instance;

public:

	struct InstanceValues
	{
		RingSingleValues viewingAngle;
		RingLocationValues outerLocation;
		RingLocationValues innerLocation;
		RingSingleValues centerRatio;
		RingColorValues outerColor;
		RingColorValues centerColor;
		RingColorValues innerColor;
	};

public:

	AlphaBlendType		AlphaBlend;
	BillboardType	Billboard;

	int32_t	VertexCount;

	RingSingleParameter	ViewingAngle;

	RingLocationParameter	OuterLocation;
	RingLocationParameter	InnerLocation;
	
	RingSingleParameter	CenterRatio;
	
	RingColorParameter OuterColor;
	RingColorParameter CenterColor;
	RingColorParameter InnerColor;

	int RingTexture;

	EffectNodeRing( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
	{
	}

	~EffectNodeRing()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	void BeginRendering(int32_t count, Manager* manager);

	void Rendering(const Instance& instance, Manager* manager);

	void EndRendering(Manager* manager);

	void InitializeRenderedInstance(Instance& instance, Manager* manager);

	void UpdateRenderedInstance(Instance& instance, Manager* manager);

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_RING; }

private:
	void LoadSingleParameter( unsigned char*& pos, RingSingleParameter& param );

	void LoadLocationParameter( unsigned char*& pos, RingLocationParameter& param );
	
	void LoadColorParameter( unsigned char*& pos, RingColorParameter& param );
	
	void InitializeSingleValues(const RingSingleParameter& param, RingSingleValues& values, Manager* manager);

	void InitializeLocationValues(const RingLocationParameter& param, RingLocationValues& values, Manager* manager);
	
	void InitializeColorValues(const RingColorParameter& param, RingColorValues& values, Manager* manager);
	
	void UpdateSingleValues( Instance& instance, const RingSingleParameter& param, RingSingleValues& values );
	
	void UpdateLocationValues( Instance& instance, const RingLocationParameter& param, RingLocationValues& values );
	
	void UpdateColorValues( Instance& instance, const RingColorParameter& param, RingColorValues& values );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_RING_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_ROOT_H__
#define	__EFFEKSEER_ParameterNODE_ROOT_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	
class EffectNodeRoot
	: public EffectNodeImplemented
{
	friend class Manager;
	friend class Effect;
	friend class Instance;

protected:

	

public:
	EffectNodeRoot( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
	{
	}

	~EffectNodeRoot()
	{
	
	}

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_ROOT; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_ROOT_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_SPRITE_H__
#define	__EFFEKSEER_ParameterNODE_SPRITE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
struct SpriteColorParameter
{
	enum
	{
		Default = 0,
		Fixed = 1,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
		
		} def;

		struct
		{
			color ll;
			color lr;
			color ul;
			color ur;
		} fixed;
	};
};

struct SpritePositionParameter
{
	enum
	{
		Default = 0,
		Fixed = 1,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
		
		} def;

		struct
		{
			vector2d ll;
			vector2d lr;
			vector2d ul;
			vector2d ur;
		} fixed;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class EffectNodeSprite
	: public EffectNodeImplemented
{
	friend class Manager;
	friend class Effect;
	friend class Instance;

public:

	struct InstanceValues
	{
		// 色
		color _color;

		union 
		{
			struct
			{
			
			} fixed;

			struct
			{

			} random;

			struct
			{
				color start;
				color  end;

			} easing;

			struct
			{
				float	offset[4];
			} fcurve_rgba;

		} allColorValues;

		union
		{
	
		} colorValues;

		union
		{
	
		} positionValues;
	};

public:

	AlphaBlendType		AlphaBlend;
	BillboardType	Billboard;

	StandardColorParameter	SpriteAllColor;

	SpriteColorParameter SpriteColor;
	SpritePositionParameter SpritePosition;

	int SpriteTexture;

	EffectNodeSprite( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	void BeginRendering(int32_t count, Manager* manager);

	void Rendering(const Instance& instance, Manager* manager);

	void EndRendering(Manager* manager);

	void InitializeRenderedInstance(Instance& instance, Manager* manager);

	void UpdateRenderedInstance(Instance& instance, Manager* manager);

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_SPRITE; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_SPRITE_H__
﻿
#ifndef	__EFFEKSEER_ParameterNODE_TRACK_H__
#define	__EFFEKSEER_ParameterNODE_TRACK_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct TrackSizeParameter
{
	enum
	{
		Fixed = 0,

		Parameter_DWORD = 0x7fffffff,
	} type;

	union
	{
		struct
		{
			float size;
		} fixed;
	};
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class EffectNodeTrack
	: public EffectNodeImplemented
{
public:

	struct InstanceGroupValues
	{
		struct Color
		{
			union 
			{
				struct
				{
					color color_;
				} fixed;

				struct
				{
					color color_;
				} random;

				struct
				{
					color start;
					color  end;
				} easing;

				struct
				{
					float	offset[4];
				} fcurve_rgba;

			} color;
		};

		struct Size
		{
			union
			{
				struct
				{
					float	size_;
				} fixed;
			} size;
		};

		Color	ColorLeft;
		Color	ColorCenter;
		Color	ColorRight;

		Color	ColorLeftMiddle;
		Color	ColorCenterMiddle;
		Color	ColorRightMiddle;

		Size	SizeFor;
		Size	SizeMiddle;
		Size	SizeBack;

	};

	struct InstanceValues
	{
		color	colorLeft;
		color	colorCenter;
		color	colorRight;

		color	colorLeftMiddle;
		color	colorCenterMiddle;
		color	colorRightMiddle;

		float	SizeFor;
		float	SizeMiddle;
		float	SizeBack;
	};

	TrackRenderer::NodeParameter	m_nodeParameter;
	TrackRenderer::InstanceParameter m_instanceParameter;

	InstanceGroupValues		m_currentGroupValues;

public:

	AlphaBlendType		AlphaBlend;

	StandardColorParameter	TrackColorLeft;
	StandardColorParameter	TrackColorCenter;
	StandardColorParameter	TrackColorRight;

	StandardColorParameter	TrackColorLeftMiddle;
	StandardColorParameter	TrackColorCenterMiddle;
	StandardColorParameter	TrackColorRightMiddle;
	
	TrackSizeParameter	TrackSizeFor;
	TrackSizeParameter	TrackSizeMiddle;
	TrackSizeParameter	TrackSizeBack;

	int TrackTexture;

	EffectNodeTrack( Effect* effect, unsigned char*& pos )
		: EffectNodeImplemented(effect, pos)
		, TrackTexture	( -1 )
	{
	}

	~EffectNodeTrack()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting);

	void BeginRendering(int32_t count, Manager* manager);

	void BeginRenderingGroup(InstanceGroup* group, Manager* manager);

	void Rendering(const Instance& instance, Manager* manager);

	void EndRendering(Manager* manager);

	void InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager);

	void InitializeRenderedInstance(Instance& instance, Manager* manager);

	void UpdateRenderedInstance(Instance& instance, Manager* manager);

	eEffectNodeType GetType() const { return EFFECT_NODE_TYPE_TRACK; }

	void InitializeValues(InstanceGroupValues::Color& value, StandardColorParameter& param, Manager* manager);
	void InitializeValues(InstanceGroupValues::Size& value, TrackSizeParameter& param, Manager* manager);
	void SetValues( Color& c, InstanceGroupValues::Color& value, StandardColorParameter& param, int32_t time, int32_t livedTime );
	void SetValues( float& s, InstanceGroupValues::Size& value, TrackSizeParameter& param, float time );
	void LoadValues( TrackSizeParameter& param, unsigned char*& pos );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_TRACK_H__
﻿
#ifndef	__EFFEKSEER_EFFECT_IMPLEMENTED_H__
#define	__EFFEKSEER_EFFECT_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	エフェクトパラメーター
	@note
	エフェクトに設定されたパラメーター。
*/

class EffectImplemented
	: public Effect
	, public ReferenceObject
{
	friend class ManagerImplemented;
private:
	ManagerImplemented* m_pManager;

	Setting*	m_setting;

	mutable std::atomic<int32_t> m_reference;

	int	m_version;

	int	m_ImageCount;
	EFK_CHAR**		m_ImagePaths;
	void**			m_pImages;

	int	m_normalImageCount;
	EFK_CHAR**		m_normalImagePaths;
	void**			m_normalImages;
	
	int	m_distortionImageCount;
	EFK_CHAR**		m_distortionImagePaths;
	void**			m_distortionImages;

	int	m_WaveCount;
	EFK_CHAR**		m_WavePaths;
	void**			m_pWaves;

	int32_t	m_modelCount;
	EFK_CHAR**		m_modelPaths;
	void**			m_pModels;

	std::basic_string<EFK_CHAR>		m_materialPath;

	/* 拡大率 */
	float	m_maginification;

	float	m_maginificationExternal;

	// 子ノード
	EffectNode* m_pRoot;

	/* カリング */
	struct
	{
		CullingShape	Shape;
		Vector3D		Location;

		union
		{
			struct
			{
			} None;

			struct
			{
				float Radius;
			} Sphere;
		};

	} Culling;


public:
	/**
		@brief	生成
	*/
	static Effect* Create( Manager* pManager, void* pData, int size, float magnification, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	生成
	*/
	static Effect* Create( Setting* setting, void* pData, int size, float magnification, const EFK_CHAR* materialPath = NULL );

	// コンストラクタ
	EffectImplemented( Manager* pManager, void* pData, int size );

	// コンストラクタ
	EffectImplemented( Setting* setting, void* pData, int size );

	// デストラクタ
	virtual ~EffectImplemented();

	// Rootの取得
	EffectNode* GetRoot() const;

	/* 拡大率の取得 */
	float GetMaginification() const;

	/**
		@brief	読み込む。
	*/
	bool Load( void* pData, int size, float mag, const EFK_CHAR* materialPath );

	/**
		@breif	何も読み込まれていない状態に戻す
	*/
	void Reset();

private:
	/**
		@brief	マネージャー取得
	*/
	Manager* GetManager() const;

public:
	/**
	@brief	設定取得
	*/
	Setting* GetSetting() const;
	
	/**
		@brief	エフェクトデータのバージョン取得
	*/
	int GetVersion() const;

	/**
		@brief	格納されている画像のポインタを取得する。
	*/
	void* GetColorImage(int n) const;

	/**
		@brief	格納されている画像のポインタの個数を取得する。
	*/
	int32_t GetColorImageCount() const;

	/**
	@brief	格納されている画像のポインタを取得する。
	*/
	void* GetNormalImage(int n) const;

	int32_t GetNormalImageCount() const;

	void* GetDistortionImage(int n) const;

	int32_t GetDistortionImageCount() const;

	/**
		@brief	格納されている音波形のポインタを取得する。
	*/
	void* GetWave( int n ) const;

	int32_t GetWaveCount() const;

	/**
		@brief	格納されているモデルのポインタを取得する。
	*/
	void* GetModel( int n ) const;

	int32_t GetModelCount() const;

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( void* data, int32_t size, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( const EFK_CHAR* path, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( Manager* managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( Manager* managers, int32_t managersCount, const EFK_CHAR* path, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	画像等リソースの再読み込みを行う。
	*/
	void ReloadResources( const EFK_CHAR* materialPath );

	/**
		@brief	画像等リソースの破棄を行う。
	*/
	void UnloadResources();

	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_EFFECT_IMPLEMENTED_H__
﻿
#ifndef	__EFFEKSEER_MANAGER_IMPLEMENTED_H__
#define	__EFFEKSEER_MANAGER_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief エフェクト管理クラス(実装)
*/
class ManagerImplemented
	: public Manager
	, public ReferenceObject
{
	friend class Effect;
	friend class EffectNode;
	friend class InstanceContainer;
	friend class InstanceGroup;

private:

	/**
		@brief	再生オブジェクトの組み合わせ
	*/
	class DrawSet
	{
	public:
		Effect*			ParameterPointer;
		InstanceContainer*	InstanceContainerPointer;
		InstanceGlobal*		GlobalPointer;
		Culling3D::Object*	CullingObjectPointer;
		bool				IsPaused;
		bool				IsShown;
		bool				IsAutoDrawing;
		bool				IsRemoving;
		bool				IsParameterChanged;
		bool				DoUseBaseMatrix;
		bool				GoingToStop;
		bool				GoingToStopRoot;
		EffectInstanceRemovingCallback	RemovingCallback;
		
		Matrix43			BaseMatrix;

		Matrix43			GlobalMatrix;

		float				Speed;

		Handle				Self;

		DrawSet( Effect* effect, InstanceContainer* pContainer, InstanceGlobal* pGlobal )
			: ParameterPointer			( effect )
			, InstanceContainerPointer	( pContainer )
			, GlobalPointer				( pGlobal )
			, CullingObjectPointer		( NULL )
			, IsPaused					( false )
			, IsShown					( true )
			, IsAutoDrawing				( true )
			, IsRemoving				( false )
			, IsParameterChanged		( false )
			, DoUseBaseMatrix			( false )
			, GoingToStop				( false )
			, GoingToStopRoot			( false )
			, RemovingCallback			( NULL )
			, Speed						( 1.0f )
			, Self						( -1 )
		{
		
		}

		DrawSet()
			: ParameterPointer			( NULL )
			, InstanceContainerPointer	( NULL )
			, GlobalPointer				( NULL )
			, CullingObjectPointer		( NULL )
			, IsPaused					( false )
			, IsShown					( true )
			, IsRemoving				( false )
			, IsParameterChanged		( false )
			, RemovingCallback			( NULL )
			, DoUseBaseMatrix			( false )
			, Speed						( 1.0f )
			, Self						( -1 )
		{
		
		}
	};

	struct CullingParameter
	{
		float		SizeX;
		float		SizeY;
		float		SizeZ;
		int32_t		LayerCount;

		CullingParameter()
		{
			SizeX = 0.0f;
			SizeY = 0.0f;
			SizeZ = 0.0f;
			LayerCount = 0;
		}

	} cullingCurrent, cullingNext;

private:
	/* 自動データ入れ替えフラグ */
	bool m_autoFlip;

	// 次のHandle
	Handle		m_NextHandle;

	// 確保済みインスタンス数
	int m_instance_max;

	// 確保済みインスタンス
	std::queue<Instance*>	m_reserved_instances;

	// 確保済みインスタンスバッファ
	uint8_t*				m_reserved_instances_buffer;

	// 再生中オブジェクトの組み合わせ集合体
	std::map<Handle,DrawSet>	m_DrawSets;

	// 破棄待ちオブジェクト
	std::map<Handle,DrawSet>	m_RemovingDrawSets[2];

	/* 描画中オブジェクト */
	std::vector<DrawSet>		m_renderingDrawSets;
	std::map<Handle,DrawSet>	m_renderingDrawSetMaps;

	/* 描画セッション */
	CriticalSection				m_renderingSession;

	/* 設定インスタンス */
	Setting*					m_setting;

	int							m_updateTime;
	int							m_drawTime;

	/* 更新回数カウント */
	uint32_t					m_sequenceNumber;
	
	/* カリング */
	Culling3D::World*			m_cullingWorld;

	/* カリング */
	std::vector<DrawSet*>	m_culledObjects;
	std::set<Handle>		m_culledObjectSets;
	bool					m_culled;

	/* スプライト描画機能用インスタンス */
	SpriteRenderer*				m_spriteRenderer;

	/* リボン描画機能用インスタンス */
	RibbonRenderer*				m_ribbonRenderer;

	/* リング描画機能用インスタンス */
	RingRenderer*				m_ringRenderer;

	/* モデル描画機能用インスタンス */
	ModelRenderer*				m_modelRenderer;

	/* トラック描画機能用インスタンス */
	TrackRenderer*				m_trackRenderer;

	/* サウンド再生用インスタンス */
	SoundPlayer*				m_soundPlayer;

	// メモリ確保関数
	MallocFunc	m_MallocFunc;

	// メモリ破棄関数
	FreeFunc	m_FreeFunc;

	// ランダム関数
	RandFunc	m_randFunc;

	// ランダム関数最大値
	int			m_randMax;

	// 描画オブジェクト追加
	Handle AddDrawSet( Effect* effect, InstanceContainer* pInstanceContainer, InstanceGlobal* pGlobalPointer );

	// 描画オブジェクト破棄処理
	void GCDrawSet( bool isRemovingManager );

	// インスタンスコンテナ生成
	InstanceContainer* CreateInstanceContainer( EffectNode* pEffectNode, InstanceGlobal* pGlobal, bool isRoot = false, Instance* pParent = NULL );

	// メモリ確保関数
	static void* EFK_STDCALL Malloc( unsigned int size );

	// メモリ破棄関数
	static void EFK_STDCALL Free( void* p, unsigned int size );

	// ランダム関数
	static int EFK_STDCALL Rand();

	// 破棄等のイベントを実際に実行
	void ExecuteEvents();
public:

	// コンストラクタ
	ManagerImplemented( int instance_max, bool autoFlip );

	// デストラクタ
	virtual ~ManagerImplemented();

	/* Root以外の破棄済みインスタンスバッファ回収(Flip,Update,終了時からのみ呼ばれる) */
	void PushInstance( Instance* instance );

	/* Root以外のインスタンスバッファ取得(Flip,Update,終了時からのみ呼ばれる) */
	Instance* PopInstance();

	/**
		@brief マネージャー破棄
		@note
		このマネージャーから生成されたエフェクトは全て強制的に破棄されます。
	*/
	void Destroy();

	/**
		@brief	更新番号を取得する。
	*/
	uint32_t GetSequenceNumber() const;

	/**
		@brief	メモリ確保関数取得
	*/
	MallocFunc GetMallocFunc() const;

	/**
		@brief	メモリ確保関数設定
	*/
	void SetMallocFunc( MallocFunc func );

	/**
		@brief	メモリ破棄関数取得
	*/
	FreeFunc GetFreeFunc() const;

	/**
		@brief	メモリ破棄関数設定
	*/
	void SetFreeFunc( FreeFunc func );

	/**
		@brief	ランダム関数取得
	*/
	RandFunc GetRandFunc() const;

	/**
		@brief	ランダム関数設定
	*/
	void SetRandFunc( RandFunc func );

	/**
		@brief	ランダム最大値取得
	*/
	int GetRandMax() const;

	/**
		@brief	ランダム関数設定
	*/
	void SetRandMax( int max_ );

	/**
		@brief	座標系を取得する。
	*/
	CoordinateSystem GetCoordinateSystem() const;

	/**
		@brief	座標系を設定する。
	*/
	void SetCoordinateSystem( CoordinateSystem coordinateSystem );

	/**
		@brief	スプライト描画機能取得
	*/
	SpriteRenderer* GetSpriteRenderer();

	/**
		@brief	スプライト描画機能設定
	*/
	void SetSpriteRenderer( SpriteRenderer* renderer );

	/**
		@brief	リボン描画機能取得
	*/
	RibbonRenderer* GetRibbonRenderer();

	/**
		@brief	リボン描画機能設定
	*/
	void SetRibbonRenderer( RibbonRenderer* renderer );

	/**
		@brief	リング描画機能取得
	*/
	RingRenderer* GetRingRenderer();

	/**
		@brief	リング描画機能設定
	*/
	void SetRingRenderer( RingRenderer* renderer );

	/**
		@brief	モデル描画機能取得
	*/
	ModelRenderer* GetModelRenderer();

	/**
		@brief	モデル描画機能設定
	*/
	void SetModelRenderer( ModelRenderer* renderer );

	/**
		@brief	軌跡描画機能取得
	*/
	TrackRenderer* GetTrackRenderer();

	/**
		@brief	軌跡描画機能設定
	*/
	void SetTrackRenderer( TrackRenderer* renderer );

	/**
		@brief	設定クラスを取得する。
	*/
	Setting* GetSetting();

	/**
		@brief	設定クラスを設定する。
		@param	setting	[in]	設定
	*/
	void SetSetting(Setting* setting);

	/**
		@brief	エフェクト読込クラスを取得する。
	*/
	EffectLoader* GetEffectLoader();

	/**
		@brief	エフェクト読込クラスを設定する。
	*/
	void SetEffectLoader( EffectLoader* effectLoader );

	/**
		@brief	テクスチャ読込クラスを取得する。
	*/
	TextureLoader* GetTextureLoader();

	/**
		@brief	テクスチャ読込クラスを設定する。
	*/
	void SetTextureLoader( TextureLoader* textureLoader );
	
	/**
		@brief	サウンド再生取得
	*/
	SoundPlayer* GetSoundPlayer();

	/**
		@brief	サウンド再生機能設定
	*/
	void SetSoundPlayer( SoundPlayer* soundPlayer );
	
	/**
		@brief	サウンド再生取得
	*/
	SoundLoader* GetSoundLoader();

	/**
		@brief	サウンド再生機能設定
	*/
	void SetSoundLoader( SoundLoader* soundLoader );

	/**
		@brief	モデル読込クラスを取得する。
	*/
	ModelLoader* GetModelLoader();

	/**
		@brief	モデル読込クラスを設定する。
	*/
	void SetModelLoader( ModelLoader* modelLoader );
	
	/**
		@brief	エフェクト停止
	*/
	void StopEffect( Handle handle );

	/**
		@brief	全てのエフェクト停止
	*/
	void StopAllEffects();

	/**
		@brief	エフェクト停止
	*/
	void StopRoot( Handle handle );

	/**
		@brief	エフェクト停止
	*/
	void StopRoot( Effect* effect );

	/**
		@brief	エフェクトのインスタンスが存在しているか取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	存在してるか?
	*/
	bool Exists( Handle handle );

	int32_t GetInstanceCount( Handle handle );

	/**
		@brief	エフェクトのインスタンスに設定されている行列を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	行列
	*/
	Matrix43 GetMatrix( Handle handle );

	/**
		@brief	エフェクトのインスタンスに変換行列を設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	mat		[in]	変換行列
	*/
	void SetMatrix( Handle handle, const Matrix43& mat );

	Vector3D GetLocation( Handle handle );
	void SetLocation( Handle handle, float x, float y, float z );
	void SetLocation( Handle handle, const Vector3D& location );
	void AddLocation( Handle handle, const Vector3D& location );

	/**
		@brief	エフェクトのインスタンスの回転角度を指定する。(ラジアン)
	*/
	void SetRotation( Handle handle, float x, float y, float z );

	/**
		@brief	エフェクトのインスタンスの任意軸周りの反時計周りの回転角度を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	axis	[in]	軸
		@param	angle	[in]	角度(ラジアン)
	*/
	void SetRotation( Handle handle, const Vector3D& axis, float angle );

	/**
		@brief	エフェクトのインスタンスの拡大率を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	x		[in]	X方向拡大率
		@param	y		[in]	Y方向拡大率
		@param	z		[in]	Z方向拡大率
	*/
	void SetScale( Handle handle, float x, float y, float z );

	// エフェクトのターゲット位置を指定する。
	void SetTargetLocation( Handle handle, float x, float y, float z );
	void SetTargetLocation( Handle handle, const Vector3D& location );

	Matrix43 GetBaseMatrix( Handle handle );

	void SetBaseMatrix( Handle handle, const Matrix43& mat );

	/**
		@brief	エフェクトのインスタンスに廃棄時のコールバックを設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	callback	[in]	コールバック
	*/
	void SetRemovingCallback( Handle handle, EffectInstanceRemovingCallback callback );

	void SetShown( Handle handle, bool shown );
	void SetPaused( Handle handle, bool paused );
	void SetSpeed( Handle handle, float speed );
	void SetAutoDrawing( Handle handle, bool autoDraw );
	void Flip();

	/**
		@brief	更新処理
	*/
	void Update( float deltaFrame );

	/**
		@brief	更新処理を開始する。
		@note
		Updateを実行する際は、実行する必要はない。
	*/
	void BeginUpdate();

	/**
		@brief	更新処理を終了する。
		@note
		Updateを実行する際は、実行する必要はない。
	*/
	void EndUpdate();

	/**
		@brief	ハンドル単位の更新を行う。
		@param	handle		[in]	ハンドル
		@param	deltaFrame	[in]	更新するフレーム数(60fps基準)
		@note
		更新する前にBeginUpdate、更新し終わった後にEndUpdateを実行する必要がある。
	*/
	void UpdateHandle( Handle handle, float deltaFrame = 1.0f );

private:
	void UpdateHandle( DrawSet& drawSet, float deltaFrame );

public:

	/**
		@brief	描画処理
	*/
	void Draw();
	
	/**
		@brief	ハンドル単位の描画処理
	*/
	void DrawHandle( Handle handle );

	/**
		@brief	再生
	*/
	Handle Play( Effect* effect, float x, float y, float z );
	
	/**
		@brief	Update処理時間を取得。
	*/
	int GetUpdateTime() const {return m_updateTime;};
	
	/**
		@brief	Draw処理時間を取得。
	*/
	int GetDrawTime() const {return m_drawTime;};

	/**
		@brief	残りの確保したインスタンス数を取得する。
	*/
	virtual int32_t GetRestInstancesCount() const { return m_reserved_instances.size(); }

	/**
		@brief	リロードを開始する。
	*/
	void BeginReloadEffect( Effect* effect );

	/**
		@brief	リロードを停止する。
	*/
	void EndReloadEffect( Effect* effect );

	/**
		@brief	エフェクトをカリングし描画負荷を減らすための空間を生成する。
		@param	xsize	X方向幅
		@param	ysize	Y方向幅
		@param	zsize	Z方向幅
		@param	layerCount	層数(大きいほどカリングの効率は上がるがメモリも大量に使用する)
	*/
	void CreateCullingWorld( float xsize, float ysize, float zsize, int32_t layerCount);

	/**
		@brief	カリングを行い、カリングされたオブジェクトのみを描画するようにする。
		@param	cameraProjMat	カメラプロジェクション行列
		@param	isOpenGL		OpenGLによる描画か?
	*/
	void CalcCulling(const Matrix44& cameraProjMat, bool isOpenGL);

	/**
		@brief	現在存在するエフェクトのハンドルからカリングの空間を配置しなおす。
	*/
	void RessignCulling() override;

	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_MANAGER_IMPLEMENTED_H__
﻿
#ifndef	__EFFEKSEER_INSTANCECONTAINER_H__
#define	__EFFEKSEER_INSTANCECONTAINER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	
	@note

*/
class InstanceContainer
{
	friend class ManagerImplemented;

private:

	// マネージャ
	Manager*	m_pManager;

	// パラメーター
	EffectNodeImplemented* m_pEffectNode;

	// グローバル
	InstanceGlobal*	m_pGlobal;

	// 子のコンテナ
	InstanceContainer**	m_Children;

	// インスタンスの子の数
	int	m_ChildrenCount;

	// グループの連結リストの先頭
	InstanceGroup*	m_headGroups;

	// グループの連結リストの最後
	InstanceGroup*	m_tailGroups;

	// placement new
	static void* operator new( size_t size, Manager* pManager );

	// placement delete
	static void operator delete( void* p, Manager* pManager );

	// default delete
	static void operator delete( void* p ){}

	// コンストラクタ
	InstanceContainer( Manager* pManager, EffectNode* pEffectNode, InstanceGlobal* pGlobal, int ChildrenCount );

	// デストラクタ
	virtual ~InstanceContainer();

	// 指定した番号にコンテナを設定
	void SetChild( int num, InstanceContainer* pContainter );

	// 無効なグループの破棄
	void RemoveInvalidGroups();

public:
	// 指定した番号のコンテナを取得
	InstanceContainer* GetChild( int num );

	/**
		@brief	グループの作成
	*/
	InstanceGroup* CreateGroup();

	/**
		@brief	グループの先頭取得
	*/
	InstanceGroup* GetFirstGroup() const;

	void Update( bool recursive, float deltaFrame, bool shown );

	void SetBaseMatrix( bool recursive, const Matrix43& mat );

	void RemoveForcibly( bool recursive );

	void Draw( bool recursive );

	void KillAllInstances(  bool recursive );

	InstanceGlobal* GetRootInstance();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INSTANCECONTAINER_H__
﻿
#ifndef	__EFFEKSEER_INSTANCE_H__
#define	__EFFEKSEER_INSTANCE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	エフェクトの実体
*/
class Instance
{
	friend class Manager;
	friend class InstanceContainer;


public:
	static const int32_t ChildrenMax = 16;

	// マネージャ
	Manager*	m_pManager;

	// パラメーター
	EffectNodeImplemented* m_pEffectNode;

	// コンテナ
	InstanceContainer*	m_pContainer;

	// グループの連結リストの先頭
	InstanceGroup*	m_headGroups;

	// 親
	Instance*	m_pParent;
	
	// グローバル位置
	Vector3D	m_GlobalPosition;
	Vector3D	m_GlobalVelocity;
	
	// グローバル位置補正
	Vector3D	m_GlobalRevisionLocation;
	Vector3D	m_GlobalRevisionVelocity;
	
	union 
	{
		struct
		{
		
		} fixed;

		struct
		{
			vector3d location;
			vector3d velocity;
			vector3d acceleration;
		} random;

		struct
		{
			vector3d	start;
			vector3d	end;
		} easing;

		struct
		{
			vector3d	offset;
		} fcruve;

	} translation_values;

	union 
	{
		struct
		{
		
		} fixed;

		struct
		{
			vector3d rotation;
			vector3d velocity;
			vector3d acceleration;
		} random;

		struct
		{
			vector3d start;
			vector3d end;
		} easing;
		
		struct
		{
			float rotation;
			vector3d axis;

			union
			{
				struct
				{
					float rotation;
					float velocity;
					float acceleration;
				} random;

				struct
				{
					float start;
					float end;
				} easing;
			};
		} axis;

		struct
		{
			vector3d offset;
		} fcruve;

	} rotation_values;

	union 
	{
		struct
		{
		
		} fixed;

		struct
		{
			vector3d  scale;
			vector3d  velocity;
			vector3d  acceleration;
		} random;

		struct
		{
			vector3d  start;
			vector3d  end;
		} easing;
		
		struct
		{
			float  scale;
			float  velocity;
			float  acceleration;
		} single_random;

		struct
		{
			float  start;
			float  end;
		} single_easing;

		struct
		{
			vector3d offset;
		} fcruve;

	} scaling_values;

	// 描画
	union
	{
		EffectNodeSprite::InstanceValues	sprite;
		EffectNodeRibbon::InstanceValues	ribbon;
		EffectNodeRing::InstanceValues		ring;
		EffectNodeModel::InstanceValues		model;
		EffectNodeTrack::InstanceValues		track;
	} rendererValues;
	
	// 音
	union
	{
		int		delay;
	} soundValues;

	// 状態
	eInstanceState	m_State;

	// 生存時間
	float		m_LivedTime;

	// 生成されてからの時間
	float		m_LivingTime;

	/* 生成された子の個数 */
	int32_t		m_generatedChildrenCount[ChildrenMax];

	/* 次に子を生成する時間 */
	float		m_nextGenerationTime[ChildrenMax];

	// 生成位置
	Matrix43		m_GenerationLocation;

	// 変換用行列
	Matrix43		m_GlobalMatrix43;

	// 親の変換用行列
	Matrix43		m_ParentMatrix43;

	/* 時間を進めるかどうか? */
	bool			m_stepTime;

	/* 更新番号 */
	uint32_t		m_sequenceNumber;

	// コンストラクタ
	Instance( Manager* pManager, EffectNode* pEffectNode, InstanceContainer* pContainer );

	// デストラクタ
	virtual ~Instance();

public:
	/**
		@brief	状態の取得
	*/
	eInstanceState GetState() const;

	/**
		@brief	行列の取得
	*/
	const Matrix43& GetGlobalMatrix43() const;

	/**
		@brief	初期化
	*/
	void Initialize( Instance* parent, int32_t instanceNumber );

	/**
		@brief	更新
	*/
	void Update( float deltaFrame, bool shown );

	/**
		@brief	描画
	*/
	void Draw();

	/**
		@brief	破棄
	*/
	void Kill();

	/**
		@brief	UVの位置取得
	*/
	RectF GetUV() const;

private:
	/**
		@brief	行列の更新
	*/
	void CalculateMatrix( float deltaFrame );
	
	/**
		@brief	行列の更新
	*/
	void CalculateParentMatrix();
	
	/**
		@brief	絶対パラメータの反映
	*/
	void ModifyMatrixFromLocationAbs( float deltaFrame );
	
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INSTANCE_H__
﻿
#ifndef	__EFFEKSEER_INSTANCEGLOBAL_H__
#define	__EFFEKSEER_INSTANCEGLOBAL_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	インスタンス共通部分
	@note
	生成されたインスタンスの全てから参照できる部分
*/
class InstanceGlobal
{
	friend class ManagerImplemented;

private:
	/* このエフェクトで使用しているインスタンス数 */
	int			m_instanceCount;
	
	/* 更新されたフレーム数 */
	float		m_updatedFrame;

	InstanceContainer*	m_rootContainer;
	Vector3D			m_targetLocation;

	InstanceGlobal();

	virtual ~InstanceGlobal();

public:
	void IncInstanceCount();

	void DecInstanceCount();

	void AddUpdatedFrame( float frame );

	/**
		@brief	全てのインスタンス数を取得
	*/
	int GetInstanceCount();

	/**
		@brief	更新されたフレーム数を取得する。
	*/
	float GetUpdatedFrame();

	InstanceContainer* GetRootContainer() const;
	void SetRootContainer( InstanceContainer* container );

	const Vector3D& GetTargetLocation() const;
	void SetTargetLocation( const Vector3D& location );
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INSTANCEGLOBAL_H__
﻿
#ifndef	__EFFEKSEER_INSTANCEGROUP_H__
#define	__EFFEKSEER_INSTANCEGROUP_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	インスタンスグループ
	@note
	インスタンスコンテナ内でさらにインスタンスをグループ化するクラス
*/
class InstanceGroup
{
friend class InstanceContainer;

private:
	ManagerImplemented*		m_manager;
	EffectNodeImplemented*	m_effectNode;
	InstanceContainer*	m_container;
	InstanceGlobal*		m_global;
	int32_t				m_time;

	// インスタンスの実体
	std::list<Instance*> m_instances;
	std::list<Instance*> m_removingInstances;

	InstanceGroup( Manager* manager, EffectNode* effectNode, InstanceContainer* container, InstanceGlobal* global );

	~InstanceGroup();

	void RemoveInvalidInstances();

public:

	/** 
		@brief	描画に必要なパラメータ
	*/
	union
	{
		EffectNodeTrack::InstanceGroupValues		track;
	} rendererValues;

	/**
		@brief	インスタンスの生成
	*/
	Instance* CreateInstance();

	Instance* GetFirst();

	int GetInstanceCount() const;

	int GetRemovingInstanceCount() const;

	void Update( float deltaFrame, bool shown );

	void SetBaseMatrix( const Matrix43& mat );

	void RemoveForcibly();

	void KillAllInstances();

	int32_t GetTime() const { return m_time; }

	/**
		@brief	グループを生成したインスタンスからの参照が残っているか?
	*/
	bool IsReferencedFromInstance;

	/**
		@brief	インスタンスから利用する連結リストの次のオブジェクトへのポインタ
	*/
	InstanceGroup*	NextUsedByInstance;

	/**
		@brief	コンテナから利用する連結リストの次のオブジェクトへのポインタ
	*/
	InstanceGroup*	NextUsedByContainer;

};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INSTANCEGROUP_H__
﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FCurve::FCurve( float defaultValue )
	: m_defaultValue	( defaultValue )
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t FCurve::Load( void* data, int32_t version )
{
	int32_t size = 0;
	uint8_t* p = (uint8_t*)data;

	memcpy( &m_start, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	memcpy( &m_end, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	memcpy( &m_offsetMax, p, sizeof(float) );
	p += sizeof(float);
	size += sizeof(float);

	memcpy( &m_offsetMin, p, sizeof(float) );
	p += sizeof(float);
	size += sizeof(float);

	memcpy( &m_offset, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	memcpy( &m_len, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	memcpy( &m_freq, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	int32_t count = 0;
	memcpy( &count, p, sizeof(int32_t) );
	p += sizeof(int32_t);
	size += sizeof(int32_t);

	for( int32_t i = 0; i < count; i++ )
	{
		float value = 0;

		memcpy( &value, p, sizeof(float) );
		p += sizeof(float);
		size += sizeof(float);

		m_keys.push_back(value);
	}

	return size;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float FCurve::GetValue( int32_t frame )
{
	if( m_keys.size() == 0 ) return m_defaultValue;

	frame -= m_offset;

	if( frame < 0 )
	{
		if( m_start == FC_EDGE_Constant )
		{
			return m_keys[0];
		}
		else if( m_start == FC_EDGE_Loop )
		{
			frame = m_len - (-frame) % m_len;
		}
		else if( m_start == FC_EDGE_LoopInversely )
		{
			frame = (-frame) % m_len;
		}
	}

	if( m_len < frame )
	{
		if( m_end == FC_EDGE_Constant )
		{
			return m_keys[m_keys.size()-1];
		}
		else if( m_end == FC_EDGE_Loop )
		{
			frame = (frame - m_len) % m_len;
		}
		else if( m_end == FC_EDGE_LoopInversely )
		{
			frame = m_len - (frame - m_len) % m_len;
		}
	}
	
	uint32_t ind = frame / m_freq;
	if( frame == m_len )
	{
		return m_keys[m_keys.size()-1];
	}
	else if( ind == m_keys.size() - 1 )
	{
		float subF = (float)(m_len-ind*m_freq);
		float subV = m_keys[ind+1] - m_keys[ind];
		return subV / (float)(subF) * (float)(frame-ind*m_freq) + m_keys[ind];
	}
	else
	{
		float subF = (float)(m_freq);
		float subV = m_keys[ind+1] - m_keys[ind];
		return subV / (float)(subF) * (float)(frame-ind*m_freq) + m_keys[ind];
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float FCurve::GetOffset( const Manager& manager ) const
{
	return (m_offsetMax - m_offsetMin) * ( (float)manager.GetRandFunc()() / (float)manager.GetRandMax() ) + m_offsetMin;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void FCurve::ChangeCoordinate()
{
	m_offsetMax *= -1.0f;
	m_offsetMin *= -1.0f;

	for( size_t i = 0; i < m_keys.size(); i++ )
	{
		m_keys[i] *= -1.0f;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void FCurve::Maginify(float value )
{
	m_offsetMax *= value;
	m_offsetMin *= value;

	for( size_t i = 0; i < m_keys.size(); i++ )
	{
		m_keys[i] *= value;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FCurveVector3D::FCurveVector3D()
	: X		( 0 )
	, Y		( 0 )
	, Z		( 0 )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t FCurveVector3D::Load( void* data, int32_t version )
{
	int32_t size = 0;
	uint8_t* p = (uint8_t*)data;

	int32_t x_size = X.Load( p, version );
	size += x_size;
	p += x_size;

	int32_t y_size = Y.Load( p, version );
	size += y_size;
	p += y_size;

	int32_t z_size = Z.Load( p, version );
	size += z_size;
	p += z_size;

	return size;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FCurveVectorColor::FCurveVectorColor()
	: R		( 255 )
	, G		( 255 )
	, B		( 255 )
	, A		( 255 )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t FCurveVectorColor::Load( void* data, int32_t version )
{
	int32_t size = 0;
	uint8_t* p = (uint8_t*)data;

	int32_t x_size = R.Load( p, version );
	size += x_size;
	p += x_size;

	int32_t y_size = G.Load( p, version );
	size += y_size;
	p += y_size;

	int32_t z_size = B.Load( p, version );
	size += z_size;
	p += z_size;

	int32_t w_size = A.Load( p, version );
	size += w_size;
	p += w_size;

	return size;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNodeImplemented::EffectNodeImplemented(Effect* effect, unsigned char*& pos)
	: m_effect		( effect )
	, IsRendered		( true )
	, SoundType			( ParameterSoundType_None )
	, RenderingOrder	( RenderingOrder_FirstCreatedInstanceIsFirst )
	, m_userData		( NULL )
	, TranslationFCurve	( NULL )
	, RotationFCurve	( NULL )
	, ScalingFCurve		( NULL )
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::LoadParameter(unsigned char*& pos, EffectNode* parent, Setting* setting)
{
	int size = 0;
	int node_type = 0;
	memcpy( &node_type, pos, sizeof(int) );
	pos += sizeof(int);

	if( node_type == -1 )
	{
		TranslationType = ParameterTranslationType_None;
		LocationAbs.type = LocationAbsParameter::None;
		RotationType = ParameterRotationType_None;
		ScalingType = ParameterScalingType_None;
		CommonValues.MaxGeneration = 1;

		GenerationLocation.EffectsRotation = 0;
		GenerationLocation.type = ParameterGenerationLocation::TYPE_POINT;
		GenerationLocation.point.location.reset();
	}
	else
	{
		memcpy( &size, pos, sizeof(int) );
		pos += sizeof(int);

		if (m_effect->GetVersion() >= 9)
		{
			assert(size == sizeof(ParameterCommonValues));
			memcpy(&CommonValues, pos, size);
			pos += size;
		}
		else
		{
			assert(size == sizeof(ParameterCommonValues_8));
			ParameterCommonValues_8 param_8;
			memcpy(&param_8, pos, size);
			pos += size;

			CommonValues.MaxGeneration = param_8.MaxGeneration;
			CommonValues.TranslationBindType = param_8.TranslationBindType;
			CommonValues.RotationBindType = param_8.RotationBindType;
			CommonValues.ScalingBindType = param_8.ScalingBindType;
			CommonValues.RemoveWhenLifeIsExtinct = param_8.RemoveWhenLifeIsExtinct;
			CommonValues.RemoveWhenParentIsRemoved = param_8.RemoveWhenParentIsRemoved;
			CommonValues.RemoveWhenChildrenIsExtinct = param_8.RemoveWhenChildrenIsExtinct;
			CommonValues.life = param_8.life;
			CommonValues.GenerationTime.max = param_8.GenerationTime;
			CommonValues.GenerationTime.min = param_8.GenerationTime;
			CommonValues.GenerationTimeOffset.max = param_8.GenerationTimeOffset;
			CommonValues.GenerationTimeOffset.min = param_8.GenerationTimeOffset;
		}
		
		memcpy( &TranslationType, pos, sizeof(int) );
		pos += sizeof(int);

		if( TranslationType == ParameterTranslationType_Fixed )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterTranslationFixed) );
			memcpy( &TranslationFixed, pos, size );
			pos += size;

			// 無効化
			if( TranslationFixed.Position.X == 0.0f &&
				TranslationFixed.Position.Y == 0.0f &&
				TranslationFixed.Position.Z == 0.0f )
			{
				TranslationType = ParameterTranslationType_None;
				EffekseerPrintDebug("LocationType Change None\n");
			}
		}
		else if( TranslationType == ParameterTranslationType_PVA )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterTranslationPVA) );
			memcpy( &TranslationPVA, pos, size );
			pos += size;
		}
		else if( TranslationType == ParameterTranslationType_Easing )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(easing_vector3d) );
			memcpy( &TranslationEasing, pos, size );
			pos += size;
		}
		else if( TranslationType == ParameterTranslationType_FCurve )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			
			TranslationFCurve = new FCurveVector3D();
			pos += TranslationFCurve->Load( pos, m_effect->GetVersion() );
		}

		/* 位置拡大処理 */
		if( m_effect->GetVersion() >= 8 )
		{
			if( TranslationType == ParameterTranslationType_Fixed )
			{
				TranslationFixed.Position *= m_effect->GetMaginification();
			}
			else if( TranslationType == ParameterTranslationType_PVA )
			{
				TranslationPVA.location.min *= m_effect->GetMaginification();
				TranslationPVA.location.max *= m_effect->GetMaginification();
				TranslationPVA.velocity.min *= m_effect->GetMaginification();
				TranslationPVA.velocity.max *= m_effect->GetMaginification();
				TranslationPVA.acceleration.min *= m_effect->GetMaginification();
				TranslationPVA.acceleration.max *= m_effect->GetMaginification();
			}
			else if( TranslationType == ParameterTranslationType_Easing )
			{
				TranslationEasing.start.min *= m_effect->GetMaginification();
				TranslationEasing.start.max *= m_effect->GetMaginification();	
				TranslationEasing.end.min *= m_effect->GetMaginification();
				TranslationEasing.end.max *= m_effect->GetMaginification();
			}
			else if( TranslationType == ParameterTranslationType_FCurve )
			{
				TranslationFCurve->X.Maginify( m_effect->GetMaginification() );
				TranslationFCurve->Y.Maginify( m_effect->GetMaginification() );
				TranslationFCurve->Z.Maginify( m_effect->GetMaginification() );
			}
		}

		memcpy( &LocationAbs.type, pos, sizeof(int) );
		pos += sizeof(int);

		/* 絶対位置 */
		if( LocationAbs.type == LocationAbsParameter::None )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == 0 );
			memcpy( &LocationAbs.none, pos, size );
			pos += size;
		}
		else if( LocationAbs.type == LocationAbsParameter::Gravity )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(vector3d) );
			memcpy( &LocationAbs.gravity, pos, size );
			pos += size;
		}
		else if( LocationAbs.type == LocationAbsParameter::AttractiveForce )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(LocationAbs.attractiveForce) );
			memcpy( &LocationAbs.attractiveForce, pos, size );
			pos += size;
		}

		/* 絶対位置拡大処理 */
		if( m_effect->GetVersion() >= 8 )
		{
			if( LocationAbs.type == LocationAbsParameter::None )
			{
			}
			else if( LocationAbs.type == LocationAbsParameter::Gravity )
			{
				LocationAbs.gravity *= m_effect->GetMaginification();
			}
		}

		memcpy( &RotationType, pos, sizeof(int) );
		pos += sizeof(int);
		EffekseerPrintDebug("RotationType %d\n", RotationType);
		if( RotationType == ParameterRotationType_Fixed )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterRotationFixed) );
			memcpy( &RotationFixed, pos, size );
			pos += size;

			// 無効化
			if( RotationFixed.Position.X == 0.0f &&
				RotationFixed.Position.Y == 0.0f &&
				RotationFixed.Position.Z == 0.0f )
			{
				RotationType = ParameterRotationType_None;
				EffekseerPrintDebug("RotationType Change None\n");
			}
		}
		else if( RotationType == ParameterRotationType_PVA )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterRotationPVA) );
			memcpy( &RotationPVA, pos, size );
			pos += size;
		}
		else if( RotationType == ParameterRotationType_Easing )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(easing_vector3d) );
			memcpy( &RotationEasing, pos, size );
			pos += size;
		}
		else if( RotationType == ParameterRotationType_AxisPVA )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterRotationAxisPVA) );
			memcpy( &RotationAxisPVA, pos, size );
			pos += size;
		}
		else if( RotationType == ParameterRotationType_AxisEasing )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterRotationAxisEasing) );
			memcpy( &RotationAxisEasing, pos, size );
			pos += size;
		}
		else if( RotationType == ParameterRotationType_FCurve )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			
			RotationFCurve = new FCurveVector3D();
			pos += RotationFCurve->Load( pos, m_effect->GetVersion() );
		}

		memcpy( &ScalingType, pos, sizeof(int) );
		pos += sizeof(int);
		EffekseerPrintDebug("ScalingType %d\n", ScalingType);
		if( ScalingType == ParameterScalingType_Fixed )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterScalingFixed) );
			memcpy( &ScalingFixed, pos, size );
			pos += size;

			// 無効化
			if( ScalingFixed.Position.X == 1.0f &&
				ScalingFixed.Position.Y == 1.0f &&
				ScalingFixed.Position.Z == 1.0f)
			{
				ScalingType = ParameterScalingType_None;
				EffekseerPrintDebug("ScalingType Change None\n");
			}
		}
		else if( ScalingType == ParameterScalingType_PVA )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterScalingPVA) );
			memcpy( &ScalingPVA, pos, size );
			pos += size;
		}
		else if( ScalingType == ParameterScalingType_Easing )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(easing_vector3d) );
			memcpy( &ScalingEasing, pos, size );
			pos += size;
		}
		else if( ScalingType == ParameterScalingType_SinglePVA )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(ParameterScalingSinglePVA) );
			memcpy( &ScalingSinglePVA, pos, size );
			pos += size;
		}
		else if( ScalingType == ParameterScalingType_SingleEasing )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			assert( size == sizeof(easing_float) );
			memcpy( &ScalingSingleEasing, pos, size );
			pos += size;
		}
		else if( ScalingType == ParameterScalingType_FCurve )
		{
			memcpy( &size, pos, sizeof(int) );
			pos += sizeof(int);
			
			ScalingFCurve = new FCurveVector3D();
			pos += ScalingFCurve->Load( pos, m_effect->GetVersion() );
			ScalingFCurve->X.SetDefaultValue( 1.0f );
			ScalingFCurve->Y.SetDefaultValue( 1.0f );
			ScalingFCurve->Z.SetDefaultValue( 1.0f );
		}

		/* 生成位置 */
		GenerationLocation.load( pos );

		/* 生成位置拡大処理*/
		if( m_effect->GetVersion() >= 8  
			/* && (this->CommonValues.ScalingBindType == BindType_NotBind || parent->GetType() == EFFECT_NODE_TYPE_ROOT)*/ )
		{
			if( GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT )
			{
				GenerationLocation.point.location.min *= m_effect->GetMaginification();
				GenerationLocation.point.location.max *= m_effect->GetMaginification();
			}
			else if( GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE )
			{
				GenerationLocation.sphere.radius.min *= m_effect->GetMaginification();
				GenerationLocation.sphere.radius.max *= m_effect->GetMaginification();
			}
			else if( GenerationLocation.type == ParameterGenerationLocation::TYPE_CIRCLE )
			{
				GenerationLocation.circle.radius.min *= m_effect->GetMaginification();
				GenerationLocation.circle.radius.max *= m_effect->GetMaginification();
			}		
		}

		// 右手系左手系変換
		if( setting->GetCoordinateSystem() == CoordinateSystem::LH )
		{
			// Translation
			if( TranslationType == ParameterTranslationType_Fixed )
			{
				TranslationFixed.Position.Z *= -1.0f;
			}
			else if( TranslationType == ParameterTranslationType_PVA )
			{
				TranslationPVA.location.max.z *= -1.0f;
				TranslationPVA.location.min.z *= -1.0f;
				TranslationPVA.velocity.max.z *= -1.0f;
				TranslationPVA.velocity.min.z *= -1.0f;
				TranslationPVA.acceleration.max.z *= -1.0f;
				TranslationPVA.acceleration.min.z *= -1.0f;
			}
			else if( TranslationType == ParameterTranslationType_Easing )
			{
				TranslationEasing.start.max.z *= -1.0f;
				TranslationEasing.start.min.z *= -1.0f;
				TranslationEasing.end.max.z *= -1.0f;
				TranslationEasing.end.min.z *= -1.0f;
			}

			// Rotation
			if( RotationType == ParameterRotationType_Fixed )
			{
				RotationFixed.Position.X *= -1.0f;
				RotationFixed.Position.Y *= -1.0f;
			}
			else if( RotationType == ParameterRotationType_PVA )
			{
				RotationPVA.rotation.max.x *= -1.0f;
				RotationPVA.rotation.min.x *= -1.0f;
				RotationPVA.rotation.max.y *= -1.0f;
				RotationPVA.rotation.min.y *= -1.0f;
				RotationPVA.velocity.max.x *= -1.0f;
				RotationPVA.velocity.min.x *= -1.0f;
				RotationPVA.velocity.max.y *= -1.0f;
				RotationPVA.velocity.min.y *= -1.0f;
				RotationPVA.acceleration.max.x *= -1.0f;
				RotationPVA.acceleration.min.x *= -1.0f;
				RotationPVA.acceleration.max.y *= -1.0f;
				RotationPVA.acceleration.min.y *= -1.0f;
			}
			else if( RotationType == ParameterRotationType_Easing )
			{
				RotationEasing.start.max.x *= -1.0f;
				RotationEasing.start.min.x *= -1.0f;
				RotationEasing.start.max.y *= -1.0f;
				RotationEasing.start.min.y *= -1.0f;
				RotationEasing.end.max.x *= -1.0f;
				RotationEasing.end.min.x *= -1.0f;
				RotationEasing.end.max.y *= -1.0f;
				RotationEasing.end.min.y *= -1.0f;
			}
			else if( RotationType == ParameterRotationType_AxisPVA )
			{
				RotationAxisPVA.axis.max.z *= -1.0f;
				RotationAxisPVA.axis.min.z *= -1.0f;
			}
			else if( RotationType == ParameterRotationType_AxisEasing )
			{
				RotationAxisEasing.axis.max.z *= -1.0f;
				RotationAxisEasing.axis.min.z *= -1.0f;
			}
			else if( RotationType == ParameterRotationType_FCurve )
			{
				RotationFCurve->X.ChangeCoordinate();
				RotationFCurve->Y.ChangeCoordinate();
			}

			// GenerationLocation
			if( GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT )
			{
			
			}
			else if( GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE )
			{
				GenerationLocation.sphere.rotation_x.max *= -1.0f;
				GenerationLocation.sphere.rotation_x.min *= -1.0f;
				GenerationLocation.sphere.rotation_y.max *= -1.0f;
				GenerationLocation.sphere.rotation_y.min *= -1.0f;
			}
		}

		if( m_effect->GetVersion() >= 3)
		{
			Texture.load( pos, m_effect->GetVersion() );

			// 拡大処理
			Texture.DistortionIntensity *= m_effect->GetMaginification();
		}
		else
		{
			Texture.reset();
		}

		LoadRendererParameter( pos, m_effect->GetSetting() );
		
		if( m_effect->GetVersion() >= 1)
		{
			// サウンド
			memcpy( &SoundType, pos, sizeof(int) );
			pos += sizeof(int);
			if( SoundType == ParameterSoundType_Use )
			{
				memcpy( &Sound.WaveId, pos, sizeof(int32_t) );
				pos += sizeof(int32_t);
				memcpy( &Sound.Volume, pos, sizeof(random_float) );
				pos += sizeof(random_float);
				memcpy( &Sound.Pitch, pos, sizeof(random_float) );
				pos += sizeof(random_float);
				memcpy( &Sound.PanType, pos, sizeof(ParameterSoundPanType) );
				pos += sizeof(ParameterSoundPanType);
				memcpy( &Sound.Pan, pos, sizeof(random_float) );
				pos += sizeof(random_float);
				memcpy( &Sound.Distance, pos, sizeof(float) );
				pos += sizeof(float);
				memcpy( &Sound.Delay, pos, sizeof(random_int) );
				pos += sizeof(random_int);
			}
		}
	}

	// ノード
	int nodeCount = 0;
	memcpy( &nodeCount, pos, sizeof(int) );
	pos += sizeof( int );
	EffekseerPrintDebug("ChildrenCount : %d\n", nodeCount );
	m_Nodes.resize( nodeCount );
	for( size_t i = 0; i < m_Nodes.size(); i++ )
	{
		m_Nodes[i] = EffectNodeImplemented::Create(m_effect, this, pos);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNodeImplemented::~EffectNodeImplemented()
{
	for( size_t i = 0; i < m_Nodes.size(); i++ )
	{
		ES_SAFE_DELETE( m_Nodes[i] );
	}

	ES_SAFE_DELETE( TranslationFCurve );
	ES_SAFE_DELETE( RotationFCurve );
	ES_SAFE_DELETE( ScalingFCurve );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::LoadOption(uint8_t*& pos)
{
	int is_rendered = 0;
	memcpy( &is_rendered, pos, sizeof(int) );
	pos += sizeof(int);

	IsRendered = is_rendered != 0;

	int count = 0;
	memcpy( &count, pos, sizeof(int) );
	pos += sizeof(int);

	for( int i = 0; i < count; i++ )
	{
		m_Nodes[i]->LoadOption( pos );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* EffectNodeImplemented::GetEffect() const
{
	return m_effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int EffectNodeImplemented::GetChildrenCount() const
{
	return (int)m_Nodes.size();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNode* EffectNodeImplemented::GetChild(int index) const
{
	if (index >= GetChildrenCount()) return NULL;
	return m_Nodes[index];
}


EffectBasicRenderParameter EffectNodeImplemented::GetBasicRenderParameter()
{
	EffectBasicRenderParameter param;
	param.ColorTextureIndex = Texture.ColorTextureIndex;
	param.AlphaBlend = Texture.AlphaBlend;
	param.Distortion = Texture.Distortion;
	param.DistortionIntensity = Texture.DistortionIntensity;
	param.FilterType = Texture.FilterType;
	param.WrapType = Texture.WrapType;
	param.ZTest = Texture.ZTest;
	param.ZWrite = Texture.ZWrite;
	return param;
}

void EffectNodeImplemented::SetBasicRenderParameter(EffectBasicRenderParameter param)
{
	Texture.ColorTextureIndex = param.ColorTextureIndex;
	Texture.AlphaBlend = param.AlphaBlend;
	Texture.Distortion = param.Distortion;
	Texture.DistortionIntensity = param.DistortionIntensity;
	Texture.FilterType = param.FilterType;
	Texture.WrapType = param.WrapType;
	Texture.ZTest = param.ZTest;
	Texture.ZWrite = param.ZWrite;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy( &type, pos, sizeof(int) );
	pos += sizeof(int);
	assert( type == GetType() );
	EffekseerPrintDebug("Renderer : None\n");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::BeginRendering(int32_t count, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::BeginRenderingGroup(InstanceGroup* group, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::Rendering(const Instance& instance, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::EndRendering(Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float EffectNodeImplemented::GetFadeAlpha(const Instance& instance)
{
	float alpha = 1.0f;

	if( Texture.FadeInType == ParameterTexture::FADEIN_ON && instance.m_LivingTime < Texture.FadeIn.Frame )
	{
		float v = 1.0f;
		Texture.FadeIn.Value.setValueToArg( 
			v,
			0.0f,
			1.0f,
			(float)instance.m_LivingTime / (float)Texture.FadeIn.Frame );

		alpha *= v;
	}

	if( Texture.FadeOutType == ParameterTexture::FADEOUT_ON && instance.m_LivingTime + Texture.FadeOut.Frame > instance.m_LivedTime )
	{
		float v = 1.0f;
		Texture.FadeOut.Value.setValueToArg( 
			v,
			1.0f,
			0.0f,
			(float)( instance.m_LivingTime + Texture.FadeOut.Frame - instance.m_LivedTime ) / (float)Texture.FadeOut.Frame );

		alpha *= v;
	}

	return alpha;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::PlaySound_(Instance& instance, SoundTag tag, Manager* manager)
{
	SoundPlayer* player = manager->GetSoundPlayer();
	if( player == NULL )
	{
		return;
	}

	if( Sound.WaveId >= 0 )
	{
		SoundPlayer::InstanceParameter parameter;
		parameter.Data = m_effect->GetWave( Sound.WaveId );
		parameter.Volume = Sound.Volume.getValue( *manager );
		parameter.Pitch = Sound.Pitch.getValue( *manager );
		parameter.Pan = Sound.Pan.getValue( *manager );
		
		parameter.Mode3D = (Sound.PanType == ParameterSoundPanType_3D);
		Vector3D::Transform( parameter.Position, 
			Vector3D(0.0f, 0.0f, 0.0f), instance.GetGlobalMatrix43() );
		parameter.Distance = Sound.Distance;

		player->Play( tag, parameter );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNodeImplemented* EffectNodeImplemented::Create(Effect* effect, EffectNode* parent, unsigned char*& pos)
{
	EffectNodeImplemented* effectnode = NULL;

	int node_type = 0;
	memcpy( &node_type, pos, sizeof(int) );

	if( node_type == EFFECT_NODE_TYPE_ROOT )
	{
		EffekseerPrintDebug("* Create : EffectNodeRoot\n");
		effectnode = new EffectNodeRoot( effect, pos );
	}
	else if( node_type == EFFECT_NODE_TYPE_NONE )
	{
		EffekseerPrintDebug("* Create : EffectNodeNone\n");
		effectnode = new EffectNodeImplemented(effect, pos);
	}
	else if( node_type == EFFECT_NODE_TYPE_SPRITE )
	{
		EffekseerPrintDebug("* Create : EffectNodeSprite\n");
		effectnode = new EffectNodeSprite( effect, pos );
	}
	else if( node_type == EFFECT_NODE_TYPE_RIBBON )
	{
		EffekseerPrintDebug("* Create : EffectNodeRibbon\n");
		effectnode = new EffectNodeRibbon( effect, pos );
	}
	else if( node_type == EFFECT_NODE_TYPE_RING )
	{
		EffekseerPrintDebug("* Create : EffectNodeRing\n");
		effectnode = new EffectNodeRing( effect, pos );
	}
	else if( node_type == EFFECT_NODE_TYPE_MODEL )
	{
		EffekseerPrintDebug("* Create : EffectNodeModel\n");
		effectnode = new EffectNodeModel( effect, pos );
	}
	else if( node_type == EFFECT_NODE_TYPE_TRACK )
	{
		EffekseerPrintDebug("* Create : EffectNodeTrack\n");
		effectnode = new EffectNodeTrack( effect, pos );
	}
	else
	{
		assert(0);
	}

	effectnode->LoadParameter( pos, parent, effect->GetSetting());

	return effectnode;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	void EffectNodeModel::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy( &type, pos, sizeof(int) );
	pos += sizeof(int);
	assert( type == GetType() );
	EffekseerPrintDebug("Renderer : Model\n");

	int32_t size = 0;

	AlphaBlend = Texture.AlphaBlend;

	if( m_effect->GetVersion() >= 7 )
	{
		float Magnification;
		memcpy( &Magnification, pos, sizeof(float) );
		pos += sizeof(float);
	}

	memcpy( &ModelIndex, pos, sizeof(int) );
	pos += sizeof(int);

	memcpy( &NormalTextureIndex, pos, sizeof(int) );
	pos += sizeof(int);
	EffekseerPrintDebug("NormalTextureIndex : %d\n", NormalTextureIndex );

	int32_t lighting;
	memcpy( &lighting, pos, sizeof(int) );
	pos += sizeof(int);
	Lighting = lighting > 0;

	memcpy( &Culling, pos, sizeof(int) );
	pos += sizeof(int);

	AllColor.load( pos, m_effect->GetVersion() );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::BeginRendering(int32_t count, Manager* manager)
{
	ModelRenderer* renderer = manager->GetModelRenderer();
	if (renderer != NULL)
	{
		ModelRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = Texture.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->BeginRendering(nodeParameter, count, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::Rendering(const Instance& instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.model;
	ModelRenderer* renderer = manager->GetModelRenderer();
	if( renderer != NULL )
	{
		ModelRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = Texture.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;


		ModelRenderer::InstanceParameter instanceParameter;
		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		instanceParameter.UV = instance.GetUV();
		
		instValues._color.setValueToArg( instanceParameter.AllColor );
		float fadeAlpha = GetFadeAlpha( instance );
		if( fadeAlpha != 1.0f )
		{
			instanceParameter.AllColor.A = (uint8_t)(instanceParameter.AllColor.A * fadeAlpha);
		}

		renderer->Rendering( nodeParameter, instanceParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::EndRendering(Manager* manager)
{
	ModelRenderer* renderer = manager->GetModelRenderer();
	if( renderer != NULL )
	{
		ModelRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = Texture.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetSetting()->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.model;

	if( AllColor.type == StandardColorParameter::Fixed )
	{
		instValues._color = AllColor.fixed.all;
	}
	else if( AllColor.type == StandardColorParameter::Random )
	{
		instValues._color = AllColor.random.all.getValue(*(manager));
	}
	else if( AllColor.type == StandardColorParameter::Easing )
	{
		instValues.allColorValues.easing.start = AllColor.easing.all.getStartValue(*(manager));
		instValues.allColorValues.easing.end = AllColor.easing.all.getEndValue(*(manager));

		float t = instance.m_LivingTime / instance.m_LivedTime;

		AllColor.easing.all.setValueToArg(
			instValues._color, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( AllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues.allColorValues.fcurve_rgba.offset[0] = AllColor.fcurve_rgba.FCurve->R.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[1] = AllColor.fcurve_rgba.FCurve->G.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[2] = AllColor.fcurve_rgba.FCurve->B.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[3] = AllColor.fcurve_rgba.FCurve->A.GetOffset(*(manager));
		
		instValues._color.r = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + AllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.g = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + AllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.b = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + AllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.a = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + AllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.model;

	if( AllColor.type == StandardColorParameter::Easing )
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		AllColor.easing.all.setValueToArg(
			instValues._color, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( AllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues._color.r = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + AllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.g = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + AllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.b = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + AllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.a = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + AllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy( &type, pos, sizeof(int) );
	pos += sizeof(int);
	assert( type == GetType() );
	EffekseerPrintDebug("Renderer : Ribbon\n");

	int32_t size = 0;

	if( m_effect->GetVersion() >= 3)
	{
		AlphaBlend = Texture.AlphaBlend;
	}
	else
	{
		memcpy( &AlphaBlend, pos, sizeof(int) );
		pos += sizeof(int);
	}

	memcpy( &ViewpointDependent, pos, sizeof(int) );
	pos += sizeof(int);
	
	memcpy( &RibbonAllColor.type, pos, sizeof(int) );
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonColorAllType : %d\n", RibbonAllColor.type );

	if( RibbonAllColor.type == RibbonAllColorParameter::Fixed )
	{
		memcpy( &RibbonAllColor.fixed, pos, sizeof(RibbonAllColor.fixed) );
		pos += sizeof(RibbonAllColor.fixed);
	}
	else if( RibbonAllColor.type == RibbonAllColorParameter::Random )
	{
		RibbonAllColor.random.all.load( m_effect->GetVersion(), pos );
	}
	else if( RibbonAllColor.type == RibbonAllColorParameter::Easing )
	{
		RibbonAllColor.easing.all.load( m_effect->GetVersion(), pos );
	}

	memcpy( &RibbonColor.type, pos, sizeof(int) );
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonColorType : %d\n", RibbonColor.type );

	if( RibbonColor.type == RibbonColor.Default )
	{
	}
	else if( RibbonColor.type == RibbonColor.Fixed )
	{
		memcpy( &RibbonColor.fixed, pos, sizeof(RibbonColor.fixed) );
		pos += sizeof(RibbonColor.fixed);
	}

	memcpy( &RibbonPosition.type, pos, sizeof(int) );
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonPosition : %d\n", RibbonPosition.type );

	if( RibbonPosition.type == RibbonPosition.Default )
	{
		if( m_effect->GetVersion() >= 8 )
		{
			memcpy( &RibbonPosition.fixed, pos, sizeof(RibbonPosition.fixed) );
			pos += sizeof(RibbonPosition.fixed);
			RibbonPosition.type = RibbonPosition.Fixed;
		}
	}
	else if( RibbonPosition.type == RibbonPosition.Fixed )
	{
		memcpy( &RibbonPosition.fixed, pos, sizeof(RibbonPosition.fixed) );
		pos += sizeof(RibbonPosition.fixed);
	}

	if( m_effect->GetVersion() >= 3)
	{
		RibbonTexture = Texture.ColorTextureIndex;
	}
	else
	{
		memcpy( &RibbonTexture, pos, sizeof(int) );
		pos += sizeof(int);
	}

	// 右手系左手系変換
	if( setting->GetCoordinateSystem() == CoordinateSystem::LH )
	{
	}

	/* 位置拡大処理 */
	if( m_effect->GetVersion() >= 8 )
	{
		if( RibbonPosition.type == RibbonPosition.Default )
		{
		}
		else if( RibbonPosition.type == RibbonPosition.Fixed )
		{
			RibbonPosition.fixed.l *= m_effect->GetMaginification();
			RibbonPosition.fixed.r *= m_effect->GetMaginification();
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::BeginRendering(int32_t count, Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if( renderer != NULL )
	{
		m_nodeParameter.AlphaBlend = AlphaBlend;
		m_nodeParameter.TextureFilter = Texture.FilterType;
		m_nodeParameter.TextureWrap = Texture.WrapType;
		m_nodeParameter.ZTest = Texture.ZTest;
		m_nodeParameter.ZWrite = Texture.ZWrite;
		m_nodeParameter.ViewpointDependent = ViewpointDependent != 0;
		m_nodeParameter.ColorTextureIndex = RibbonTexture;
		m_nodeParameter.EffectPointer = GetEffect();

		m_nodeParameter.Distortion = Texture.Distortion;
		m_nodeParameter.DistortionIntensity = Texture.DistortionIntensity;


		renderer->BeginRendering( m_nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::BeginRenderingGroup(InstanceGroup* group, Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if( renderer != NULL )
	{
		m_instanceParameter.InstanceCount = group->GetInstanceCount();
		m_instanceParameter.InstanceIndex = 0;

		if (group->GetFirst() != nullptr)
		{
			m_instanceParameter.UV = group->GetFirst()->GetUV();
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::Rendering(const Instance& instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.ribbon;
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if( renderer != NULL )
	{
		instValues._color.setValueToArg( m_instanceParameter.AllColor );
		m_instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		color color_l = instValues._color;
		color color_r = instValues._color;

		if( RibbonColor.type == RibbonColorParameter::Default )
		{

		}
		else if( RibbonColor.type == RibbonColorParameter::Fixed )
		{
			color_l = color::mul( color_l, RibbonColor.fixed.l );
			color_r = color::mul( color_r, RibbonColor.fixed.r );
		}

		float fadeAlpha = GetFadeAlpha( instance );
		if( fadeAlpha != 1.0f )
		{
			color_l.a = (uint8_t)(color_l.a * fadeAlpha);
			color_r.a = (uint8_t)(color_r.a * fadeAlpha);
		}

		color_l.setValueToArg( m_instanceParameter.Colors[0] );
		color_r.setValueToArg( m_instanceParameter.Colors[1] );


		
		if( RibbonPosition.type == RibbonPositionParameter::Default )
		{
			m_instanceParameter.Positions[0] = -0.5f;
			m_instanceParameter.Positions[1] = 0.5f;
		}
		else if( RibbonPosition.type == RibbonPositionParameter::Fixed )
		{
			m_instanceParameter.Positions[0] = RibbonPosition.fixed.l;
			m_instanceParameter.Positions[1] = RibbonPosition.fixed.r;
		}

		renderer->Rendering( m_nodeParameter, m_instanceParameter, m_userData );

		m_instanceParameter.InstanceIndex++;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::EndRendering(Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if( renderer != NULL )
	{
		renderer->EndRendering( m_nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ribbon;

	if( RibbonAllColor.type == RibbonAllColorParameter::Fixed )
	{
		instValues._color = RibbonAllColor.fixed.all;
	}
	else if( RibbonAllColor.type == RibbonAllColorParameter::Random )
	{
		instValues._color = RibbonAllColor.random.all.getValue(*(manager));
	}
	else if( RibbonAllColor.type == RibbonAllColorParameter::Easing )
	{
		instValues.allColorValues.easing.start = RibbonAllColor.easing.all.getStartValue(*(manager));
		instValues.allColorValues.easing.end = RibbonAllColor.easing.all.getEndValue(*(manager));
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ribbon;

	if( RibbonAllColor.type == RibbonAllColorParameter::Easing )
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		RibbonAllColor.easing.all.setValueToArg(
			instValues._color, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy( &type, pos, sizeof(int) );
	pos += sizeof(int);
	assert( type == GetType() );
	EffekseerPrintDebug("Renderer : Ring\n");

	int32_t size = 0;

	memcpy( &RenderingOrder, pos, sizeof(int) );
	pos += sizeof(int);

	if( m_effect->GetVersion() >= 3)
	{
		AlphaBlend = Texture.AlphaBlend;
	}
	else
	{
		memcpy( &AlphaBlend, pos, sizeof(int) );
		pos += sizeof(int);
	}

	memcpy( &Billboard, pos, sizeof(int) );
	pos += sizeof(int);
	
	memcpy( &VertexCount, pos, sizeof(int) );
	pos += sizeof(int);
	
	LoadSingleParameter( pos, ViewingAngle );

	LoadLocationParameter( pos, OuterLocation );
	
	LoadLocationParameter( pos, InnerLocation );
	
	LoadSingleParameter( pos, CenterRatio );

	LoadColorParameter( pos, OuterColor);

	LoadColorParameter( pos, CenterColor);

	LoadColorParameter( pos, InnerColor);

	if( m_effect->GetVersion() >= 3)
	{
		RingTexture = Texture.ColorTextureIndex;
	}
	else
	{
		memcpy( &RingTexture, pos, sizeof(int) );
		pos += sizeof(int);
	}
	
	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
		if( OuterLocation.type == RingLocationParameter::Fixed )
		{
			OuterLocation.fixed.location.y *= -1;
		}
		else if( OuterLocation.type == RingLocationParameter::PVA )
		{
			OuterLocation.pva.location.min.y *= -1;
			OuterLocation.pva.location.max.y *= -1;
			OuterLocation.pva.velocity.min.y *= -1;
			OuterLocation.pva.velocity.max.y *= -1;
			OuterLocation.pva.acceleration.min.y *= -1;
			OuterLocation.pva.acceleration.max.y *= -1;
		}
		else if( OuterLocation.type == RingLocationParameter::Easing )
		{
			OuterLocation.easing.start.min.y *= -1;
			OuterLocation.easing.start.max.y *= -1;
			OuterLocation.easing.end.min.y *= -1;
			OuterLocation.easing.end.max.y *= -1;
		}

		if( InnerLocation.type == RingLocationParameter::Fixed )
		{
			InnerLocation.fixed.location.y *= -1;
		}
		else if( InnerLocation.type == RingLocationParameter::PVA )
		{
			InnerLocation.pva.location.min.y *= -1;
			InnerLocation.pva.location.max.y *= -1;
			InnerLocation.pva.velocity.min.y *= -1;
			InnerLocation.pva.velocity.max.y *= -1;
			InnerLocation.pva.acceleration.min.y *= -1;
			InnerLocation.pva.acceleration.max.y *= -1;
		}
		else if( InnerLocation.type == RingLocationParameter::Easing )
		{
			InnerLocation.easing.start.min.y *= -1;
			InnerLocation.easing.start.max.y *= -1;
			InnerLocation.easing.end.min.y *= -1;
			InnerLocation.easing.end.max.y *= -1;
		}
	}

	/* 位置拡大処理 */
	if( m_effect->GetVersion() >= 8 )
	{
		if( OuterLocation.type == RingLocationParameter::Fixed )
		{
			OuterLocation.fixed.location *= m_effect->GetMaginification();
		}
		else if( OuterLocation.type == RingLocationParameter::PVA )
		{
			OuterLocation.pva.location.min *= m_effect->GetMaginification();
			OuterLocation.pva.location.max *= m_effect->GetMaginification();
			OuterLocation.pva.velocity.min *= m_effect->GetMaginification();
			OuterLocation.pva.velocity.max *= m_effect->GetMaginification();
			OuterLocation.pva.acceleration.min *= m_effect->GetMaginification();
			OuterLocation.pva.acceleration.max *= m_effect->GetMaginification();
		}
		else if( OuterLocation.type == RingLocationParameter::Easing )
		{
			OuterLocation.easing.start.min *= m_effect->GetMaginification();
			OuterLocation.easing.start.max *= m_effect->GetMaginification();
			OuterLocation.easing.end.min *= m_effect->GetMaginification();
			OuterLocation.easing.end.max *= m_effect->GetMaginification();
		}

		if( InnerLocation.type == RingLocationParameter::Fixed )
		{
			InnerLocation.fixed.location *= m_effect->GetMaginification();
		}
		else if( InnerLocation.type == RingLocationParameter::PVA )
		{
			InnerLocation.pva.location.min *= m_effect->GetMaginification();
			InnerLocation.pva.location.max *= m_effect->GetMaginification();
			InnerLocation.pva.velocity.min *= m_effect->GetMaginification();
			InnerLocation.pva.velocity.max *= m_effect->GetMaginification();
			InnerLocation.pva.acceleration.min *= m_effect->GetMaginification();
			InnerLocation.pva.acceleration.max *= m_effect->GetMaginification();
		}
		else if( InnerLocation.type == RingLocationParameter::Easing )
		{
			InnerLocation.easing.start.min *= m_effect->GetMaginification();
			InnerLocation.easing.start.max *= m_effect->GetMaginification();
			InnerLocation.easing.end.min *= m_effect->GetMaginification();
			InnerLocation.easing.end.max *= m_effect->GetMaginification();
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::BeginRendering(int32_t count, Manager* manager)
{
	RingRenderer* renderer = manager->GetRingRenderer();
	if( renderer != NULL )
	{
		RingRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = RingTexture;
		nodeParameter.VertexCount = VertexCount;
		nodeParameter.EffectPointer = GetEffect();

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->BeginRendering( nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::Rendering(const Instance& instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.ring;
	RingRenderer* renderer = manager->GetRingRenderer();
	if( renderer != NULL )
	{
		RingRenderer::NodeParameter nodeParameter;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.VertexCount = VertexCount;
		nodeParameter.ColorTextureIndex = RingTexture;

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		RingRenderer::InstanceParameter instanceParameter;
		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		instanceParameter.ViewingAngle = instValues.viewingAngle.current;
		
		instValues.outerLocation.current.setValueToArg( instanceParameter.OuterLocation );
		instValues.innerLocation.current.setValueToArg( instanceParameter.InnerLocation );

		instanceParameter.CenterRatio = instValues.centerRatio.current;

		instValues.outerColor.current.setValueToArg( instanceParameter.OuterColor );
		instValues.centerColor.current.setValueToArg( instanceParameter.CenterColor );
		instValues.innerColor.current.setValueToArg( instanceParameter.InnerColor );
		
		float fadeAlpha = GetFadeAlpha( instance );
		if( fadeAlpha != 1.0f )
		{
			instanceParameter.OuterColor.A = (uint8_t)(instanceParameter.OuterColor.A * fadeAlpha);
			instanceParameter.CenterColor.A = (uint8_t)(instanceParameter.CenterColor.A * fadeAlpha);
			instanceParameter.InnerColor.A = (uint8_t)(instanceParameter.InnerColor.A * fadeAlpha);
		}

		instanceParameter.UV = instance.GetUV();
		renderer->Rendering( nodeParameter, instanceParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::EndRendering(Manager* manager)
{
	RingRenderer* renderer = manager->GetRingRenderer();
	if( renderer != NULL )
	{
		RingRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = RingTexture;
		nodeParameter.EffectPointer = GetEffect();

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ring;

	InitializeSingleValues(ViewingAngle, instValues.viewingAngle, manager);

	InitializeLocationValues(OuterLocation, instValues.outerLocation, manager);
	InitializeLocationValues(InnerLocation, instValues.innerLocation, manager);
	
	InitializeSingleValues(CenterRatio, instValues.centerRatio, manager);

	InitializeColorValues(OuterColor, instValues.outerColor, manager);
	InitializeColorValues(CenterColor, instValues.centerColor, manager);
	InitializeColorValues(InnerColor, instValues.innerColor, manager);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ring;
	
	UpdateSingleValues( instance, ViewingAngle, instValues.viewingAngle );

	UpdateLocationValues( instance, OuterLocation, instValues.outerLocation );
	UpdateLocationValues( instance, InnerLocation, instValues.innerLocation );
	
	UpdateSingleValues( instance, CenterRatio, instValues.centerRatio );

	UpdateColorValues( instance, OuterColor, instValues.outerColor );
	UpdateColorValues( instance, CenterColor, instValues.centerColor );
	UpdateColorValues( instance, InnerColor, instValues.innerColor );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::LoadSingleParameter( unsigned char*& pos, RingSingleParameter& param )
{
	memcpy( &param.type, pos, sizeof(int) );
	pos += sizeof(int);

	if( param.type == RingSingleParameter::Fixed )
	{
		memcpy( &param.fixed, pos, sizeof(float) );
		pos += sizeof(float);
	}
	else if( param.type == RingSingleParameter::Random )
	{
		memcpy( &param.random, pos, sizeof(param.random) );
		pos += sizeof(param.random);
	}
	else if( param.type == RingSingleParameter::Easing )
	{
		memcpy( &param.easing, pos, sizeof(param.easing) );
		pos += sizeof(param.easing);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::LoadLocationParameter( unsigned char*& pos, RingLocationParameter& param )
{
	memcpy( &param.type, pos, sizeof(int) );
	pos += sizeof(int);
	
	if( param.type == RingLocationParameter::Fixed )
	{
		memcpy( &param.fixed, pos, sizeof(param.fixed) );
		pos += sizeof(param.fixed);
	}
	else if( param.type == RingLocationParameter::PVA )
	{
		memcpy( &param.pva, pos, sizeof(param.pva) );
		pos += sizeof(param.pva);
	}
	else if( param.type == RingLocationParameter::Easing )
	{
		memcpy( &param.easing, pos, sizeof(param.easing) );
		pos += sizeof(param.easing);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::LoadColorParameter( unsigned char*& pos, RingColorParameter& param )
{
	memcpy( &param.type, pos, sizeof(int) );
	pos += sizeof(int);
	
	if( param.type == RingColorParameter::Fixed )
	{
		memcpy( &param.fixed, pos, sizeof(param.fixed) );
		pos += sizeof(param.fixed);
	}
	else if( param.type == RingColorParameter::Random )
	{
		param.random.load( m_effect->GetVersion(), pos );
	}
	else if( param.type == RingColorParameter::Easing )
	{
		param.easing.load( m_effect->GetVersion(), pos );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeSingleValues(const RingSingleParameter& param, RingSingleValues& values, Manager* manager)
{
	switch( param.type )
	{
		case RingSingleParameter::Fixed:
			values.current = param.fixed;
			break;
		case RingSingleParameter::Random:
			values.current = param.random.getValue( *manager );
			break;
		case RingSingleParameter::Easing:
			values.easing.start = param.easing.start.getValue( *manager );
			values.easing.end = param.easing.end.getValue( *manager );
			values.current = values.easing.start;
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeLocationValues(const RingLocationParameter& param, RingLocationValues& values, Manager* manager)
{
	switch( param.type )
	{
		case RingLocationParameter::Fixed:
			values.current = param.fixed.location;
			break;
		case RingLocationParameter::PVA:
			values.pva.start = param.pva.location.getValue( *manager );
			values.pva.velocity = param.pva.velocity.getValue( *manager );
			values.pva.acceleration = param.pva.acceleration.getValue( *manager );
			values.current = values.pva.start;
			break;
		case RingLocationParameter::Easing:
			values.easing.start = param.easing.start.getValue( *manager );
			values.easing.end = param.easing.end.getValue( *manager );
			values.current = values.easing.start;
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeColorValues(const RingColorParameter& param, RingColorValues& values, Manager* manager)
{
	switch( param.type )
	{
		case RingColorParameter::Fixed:
			values.current = param.fixed;
			break;
		case RingColorParameter::Random:
			values.current = param.random.getValue( *manager );
			break;
		case RingColorParameter::Easing:
			values.easing.start = param.easing.getStartValue( *manager );
			values.easing.end = param.easing.getEndValue( *manager );
			values.current = values.easing.start;
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::UpdateSingleValues( Instance& instance, const RingSingleParameter& param, RingSingleValues& values )
{
	if( param.type == RingSingleParameter::Easing )
	{
		param.easing.setValueToArg(
			values.current,
			values.easing.start,
			values.easing.end,
			instance.m_LivingTime / instance.m_LivedTime );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::UpdateLocationValues( Instance& instance, const RingLocationParameter& param, RingLocationValues& values )
{
	if( param.type == RingLocationParameter::PVA )
	{
		values.current = values.pva.start +
			values.pva.velocity * instance.m_LivingTime +
			values.pva.acceleration * instance.m_LivingTime * instance.m_LivingTime * 0.5f;
	}
	else if( param.type == RingLocationParameter::Easing )
	{
		param.easing.setValueToArg(
			values.current,
			values.easing.start,
			values.easing.end,
			instance.m_LivingTime / instance.m_LivedTime );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::UpdateColorValues( Instance& instance, const RingColorParameter& param, RingColorValues& values )
{
	if( param.type == RingColorParameter::Easing )
	{
		param.easing.setValueToArg(
			values.current, 
			values.easing.start,
			values.easing.end,
			instance.m_LivingTime / instance.m_LivedTime );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	void EffectNodeSprite::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy(&type, pos, sizeof(int));
	pos += sizeof(int);
	assert(type == GetType());
	EffekseerPrintDebug("Renderer : Sprite\n");

	int32_t size = 0;

	memcpy(&RenderingOrder, pos, sizeof(int));
	pos += sizeof(int);

	if (m_effect->GetVersion() >= 3)
	{
		AlphaBlend = Texture.AlphaBlend;
	}
	else
	{
		memcpy(&AlphaBlend, pos, sizeof(int));
		pos += sizeof(int);
	}

	memcpy(&Billboard, pos, sizeof(int));
	pos += sizeof(int);

	SpriteAllColor.load(pos, m_effect->GetVersion());
	EffekseerPrintDebug("SpriteColorAllType : %d\n", SpriteAllColor.type);

	memcpy(&SpriteColor.type, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("SpriteColorType : %d\n", SpriteColor.type);

	if (SpriteColor.type == SpriteColor.Default)
	{
	}
	else if (SpriteColor.type == SpriteColor.Fixed)
	{
		memcpy(&SpriteColor.fixed, pos, sizeof(SpriteColor.fixed));
		pos += sizeof(SpriteColor.fixed);
	}

	memcpy(&SpritePosition.type, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("SpritePosition : %d\n", SpritePosition.type);

	if (SpritePosition.type == SpritePosition.Default)
	{
		if (m_effect->GetVersion() >= 8)
		{
			memcpy(&SpritePosition.fixed, pos, sizeof(SpritePosition.fixed));
			pos += sizeof(SpritePosition.fixed);
			SpritePosition.type = SpritePosition.Fixed;
		}
	}
	else if (SpritePosition.type == SpritePosition.Fixed)
	{
		memcpy(&SpritePosition.fixed, pos, sizeof(SpritePosition.fixed));
		pos += sizeof(SpritePosition.fixed);
	}

	if (m_effect->GetVersion() >= 3)
	{
		SpriteTexture = Texture.ColorTextureIndex;
	}
	else
	{
		memcpy(&SpriteTexture, pos, sizeof(int));
		pos += sizeof(int);
	}

	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
	}

	/* 位置拡大処理 */
	if (m_effect->GetVersion() >= 8)
	{
		if (SpritePosition.type == SpritePosition.Default)
		{
		}
		else if (SpritePosition.type == SpritePosition.Fixed)
		{
			SpritePosition.fixed.ll *= m_effect->GetMaginification();
			SpritePosition.fixed.lr *= m_effect->GetMaginification();
			SpritePosition.fixed.ul *= m_effect->GetMaginification();
			SpritePosition.fixed.ur *= m_effect->GetMaginification();
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::BeginRendering(int32_t count, Manager* manager)
{
	SpriteRenderer* renderer = manager->GetSpriteRenderer();
	if( renderer != NULL )
	{
		SpriteRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();
		
		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->BeginRendering( nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::Rendering(const Instance& instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.sprite;
	SpriteRenderer* renderer = manager->GetSpriteRenderer();
	if( renderer != NULL )
	{
		SpriteRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		SpriteRenderer::InstanceParameter instanceParameter;
		instValues._color.setValueToArg( instanceParameter.AllColor );
		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		color color_ll = instValues._color;
		color color_lr = instValues._color;
		color color_ul = instValues._color;
		color color_ur = instValues._color;

		if( SpriteColor.type == SpriteColorParameter::Default )
		{
		}
		else if( SpriteColor.type == SpriteColorParameter::Fixed )
		{
			color_ll = color::mul( color_ll, SpriteColor.fixed.ll );
			color_lr = color::mul( color_lr, SpriteColor.fixed.lr );
			color_ul = color::mul( color_ul, SpriteColor.fixed.ul );
			color_ur = color::mul( color_ur, SpriteColor.fixed.ur );
		}

		float fadeAlpha = GetFadeAlpha( instance );
		if( fadeAlpha != 1.0f )
		{
			color_ll.a = (uint8_t)(color_ll.a * fadeAlpha);
			color_lr.a = (uint8_t)(color_lr.a * fadeAlpha);
			color_ul.a = (uint8_t)(color_ul.a * fadeAlpha);
			color_ur.a = (uint8_t)(color_ur.a * fadeAlpha);
		}

		color_ll.setValueToArg( instanceParameter.Colors[0] );
		color_lr.setValueToArg( instanceParameter.Colors[1] );
		color_ul.setValueToArg( instanceParameter.Colors[2] );
		color_ur.setValueToArg( instanceParameter.Colors[3] );
		

		if( SpritePosition.type == SpritePosition.Default )
		{
			instanceParameter.Positions[0].X = -0.5f;
			instanceParameter.Positions[0].Y = -0.5f;
			instanceParameter.Positions[1].X = 0.5f;
			instanceParameter.Positions[1].Y = -0.5f;
			instanceParameter.Positions[2].X = -0.5f;
			instanceParameter.Positions[2].Y = 0.5f;
			instanceParameter.Positions[3].X = 0.5f;
			instanceParameter.Positions[3].Y = 0.5f;
		}
		else if( SpritePosition.type == SpritePosition.Fixed )
		{
			SpritePosition.fixed.ll.setValueToArg( instanceParameter.Positions[0] );
			SpritePosition.fixed.lr.setValueToArg( instanceParameter.Positions[1] );
			SpritePosition.fixed.ul.setValueToArg( instanceParameter.Positions[2] );
			SpritePosition.fixed.ur.setValueToArg( instanceParameter.Positions[3] );
		}

		instanceParameter.UV = instance.GetUV();
		renderer->Rendering( nodeParameter, instanceParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::EndRendering(Manager* manager)
{
	SpriteRenderer* renderer = manager->GetSpriteRenderer();
	if( renderer != NULL )
	{
		SpriteRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = Texture.FilterType;
		nodeParameter.TextureWrap = Texture.WrapType;
		nodeParameter.ZTest = Texture.ZTest;
		nodeParameter.ZWrite = Texture.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();

		nodeParameter.Distortion = Texture.Distortion;
		nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.sprite;

	if( SpriteAllColor.type == StandardColorParameter::Fixed )
	{
		instValues._color = SpriteAllColor.fixed.all;
	}
	else if( SpriteAllColor.type == StandardColorParameter::Random )
	{
		instValues._color = SpriteAllColor.random.all.getValue(*(manager));
	}
	else if( SpriteAllColor.type == StandardColorParameter::Easing )
	{
		instValues.allColorValues.easing.start = SpriteAllColor.easing.all.getStartValue(*(manager));
		instValues.allColorValues.easing.end = SpriteAllColor.easing.all.getEndValue(*(manager));

		float t = instance.m_LivingTime / instance.m_LivedTime;

		SpriteAllColor.easing.all.setValueToArg(
			instValues._color, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( SpriteAllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues.allColorValues.fcurve_rgba.offset[0] = SpriteAllColor.fcurve_rgba.FCurve->R.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[1] = SpriteAllColor.fcurve_rgba.FCurve->G.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[2] = SpriteAllColor.fcurve_rgba.FCurve->B.GetOffset(*(manager));
		instValues.allColorValues.fcurve_rgba.offset[3] = SpriteAllColor.fcurve_rgba.FCurve->A.GetOffset(*(manager));
		
		instValues._color.r = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + SpriteAllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.g = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + SpriteAllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.b = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + SpriteAllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.a = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + SpriteAllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.sprite;

	if( SpriteAllColor.type == StandardColorParameter::Easing )
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		SpriteAllColor.easing.all.setValueToArg(
			instValues._color, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( SpriteAllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues._color.r = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + SpriteAllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.g = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + SpriteAllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.b = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + SpriteAllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._color.a = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + SpriteAllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy( &type, pos, sizeof(int) );
	pos += sizeof(int);
	assert( type == GetType() );
	EffekseerPrintDebug("Renderer : Track\n");

	int32_t size = 0;

	
	LoadValues( TrackSizeFor, pos );
	LoadValues( TrackSizeMiddle, pos );
	LoadValues( TrackSizeBack, pos );

	TrackColorLeft.load( pos, m_effect->GetVersion() );
	TrackColorLeftMiddle.load( pos, m_effect->GetVersion() );

	TrackColorCenter.load( pos, m_effect->GetVersion() );
	TrackColorCenterMiddle.load( pos, m_effect->GetVersion() );

	TrackColorRight.load( pos, m_effect->GetVersion() );
	TrackColorRightMiddle.load( pos, m_effect->GetVersion() );

	AlphaBlend = Texture.AlphaBlend;
	TrackTexture = Texture.ColorTextureIndex;

	EffekseerPrintDebug("TrackColorLeft : %d\n", TrackColorLeft.type );
	EffekseerPrintDebug("TrackColorLeftMiddle : %d\n", TrackColorLeftMiddle.type );
	EffekseerPrintDebug("TrackColorCenter : %d\n", TrackColorCenter.type );
	EffekseerPrintDebug("TrackColorCenterMiddle : %d\n", TrackColorCenterMiddle.type );
	EffekseerPrintDebug("TrackColorRight : %d\n", TrackColorRight.type );
	EffekseerPrintDebug("TrackColorRightMiddle : %d\n", TrackColorRightMiddle.type );

	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
	}

	/* 位置拡大処理 */
	if( m_effect->GetVersion() >= 8 )
	{
		TrackSizeFor.fixed.size *= m_effect->GetMaginification();
		TrackSizeMiddle.fixed.size *= m_effect->GetMaginification();
		TrackSizeBack.fixed.size *= m_effect->GetMaginification();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::BeginRendering(int32_t count, Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if( renderer != NULL )
	{
		m_nodeParameter.AlphaBlend = AlphaBlend;
		m_nodeParameter.TextureFilter = Texture.FilterType;
		m_nodeParameter.TextureWrap = Texture.WrapType;
		m_nodeParameter.ZTest = Texture.ZTest;
		m_nodeParameter.ZWrite = Texture.ZWrite;
		m_nodeParameter.ColorTextureIndex = TrackTexture;
		m_nodeParameter.EffectPointer = GetEffect();

		m_nodeParameter.Distortion = Texture.Distortion;
		m_nodeParameter.DistortionIntensity = Texture.DistortionIntensity;

		renderer->BeginRendering( m_nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::BeginRenderingGroup(InstanceGroup* group, Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if( renderer != NULL )
	{
		InstanceGroupValues& instValues = group->rendererValues.track;
		m_currentGroupValues = group->rendererValues.track;

		m_instanceParameter.InstanceCount = group->GetInstanceCount();
		m_instanceParameter.InstanceIndex = 0;

		if (group->GetFirst() != nullptr)
		{
			m_instanceParameter.UV = group->GetFirst()->GetUV();
		}
		
		/*
		SetValues( m_instanceParameter.ColorLeft, instValues.ColorLeft, TrackColorLeft, group->GetTime() );
		SetValues( m_instanceParameter.ColorCenter,instValues.ColorCenter, TrackColorCenter, group->GetTime() );
		SetValues( m_instanceParameter.ColorRight,instValues.ColorRight, TrackColorRight, group->GetTime() );

		SetValues( m_instanceParameter.ColorLeftMiddle,instValues.ColorLeftMiddle, TrackColorLeftMiddle, group->GetTime() );
		SetValues( m_instanceParameter.ColorCenterMiddle,instValues.ColorCenterMiddle, TrackColorCenterMiddle, group->GetTime() );
		SetValues( m_instanceParameter.ColorRightMiddle,instValues.ColorRightMiddle, TrackColorRightMiddle, group->GetTime() );
	
		SetValues( m_instanceParameter.SizeFor, instValues.SizeFor, TrackSizeFor, group->GetTime() );
		SetValues( m_instanceParameter.SizeMiddle, instValues.SizeMiddle, TrackSizeMiddle, group->GetTime() );
		SetValues( m_instanceParameter.SizeBack, instValues.SizeBack, TrackSizeBack, group->GetTime() );
		*/
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::Rendering(const Instance& instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.track;

	TrackRenderer* renderer = manager->GetTrackRenderer();
	if( renderer != NULL )
	{
		float t = (float)instance.m_LivingTime / (float)instance.m_LivedTime;
		int32_t time = instance.m_LivingTime;
		int32_t livedTime = instance.m_LivedTime;

		SetValues( m_instanceParameter.ColorLeft, m_currentGroupValues.ColorLeft, TrackColorLeft, time, livedTime );
		SetValues( m_instanceParameter.ColorCenter, m_currentGroupValues.ColorCenter, TrackColorCenter, time, livedTime );
		SetValues( m_instanceParameter.ColorRight, m_currentGroupValues.ColorRight, TrackColorRight, time, livedTime );

		SetValues( m_instanceParameter.ColorLeftMiddle, m_currentGroupValues.ColorLeftMiddle, TrackColorLeftMiddle, time, livedTime );
		SetValues( m_instanceParameter.ColorCenterMiddle, m_currentGroupValues.ColorCenterMiddle, TrackColorCenterMiddle, time, livedTime );
		SetValues( m_instanceParameter.ColorRightMiddle, m_currentGroupValues.ColorRightMiddle, TrackColorRightMiddle, time, livedTime );
	
		SetValues( m_instanceParameter.SizeFor, m_currentGroupValues.SizeFor, TrackSizeFor, t );
		SetValues( m_instanceParameter.SizeMiddle, m_currentGroupValues.SizeMiddle, TrackSizeMiddle, t );
		SetValues( m_instanceParameter.SizeBack, m_currentGroupValues.SizeBack, TrackSizeBack, t );

		m_instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		renderer->Rendering( m_nodeParameter, m_instanceParameter, m_userData );
		m_instanceParameter.InstanceIndex++;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::EndRendering(Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if( renderer != NULL )
	{
		renderer->EndRendering( m_nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager)
{
	InstanceGroupValues& instValues = instanceGroup.rendererValues.track;

	InitializeValues(instValues.ColorLeft, TrackColorLeft, manager);
	InitializeValues(instValues.ColorCenter, TrackColorCenter, manager);
	InitializeValues(instValues.ColorRight, TrackColorRight, manager);

	InitializeValues(instValues.ColorLeftMiddle, TrackColorLeftMiddle, manager);
	InitializeValues(instValues.ColorCenterMiddle, TrackColorCenterMiddle, manager);
	InitializeValues(instValues.ColorRightMiddle, TrackColorRightMiddle, manager);

	InitializeValues(instValues.SizeFor, TrackSizeFor, manager);
	InitializeValues(instValues.SizeBack, TrackSizeBack, manager);
	InitializeValues(instValues.SizeMiddle, TrackSizeMiddle, manager);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.track;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.track;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeValues(InstanceGroupValues::Color& value, StandardColorParameter& param, Manager* manager)
{
	if( param.type == StandardColorParameter::Fixed )
	{
		value.color.fixed.color_ = param.fixed.all;
	}
	else if( param.type == StandardColorParameter::Random )
	{
		value.color.random.color_ = param.random.all.getValue(*(manager));
	}
	else if( param.type == StandardColorParameter::Easing )
	{
		value.color.easing.start = param.easing.all.getStartValue(*(manager));
		value.color.easing.end = param.easing.all.getEndValue(*(manager));
	}
	else if( param.type == StandardColorParameter::FCurve_RGBA )
	{
		value.color.fcurve_rgba.offset[0] = param.fcurve_rgba.FCurve->R.GetOffset(*(manager));
		value.color.fcurve_rgba.offset[1] = param.fcurve_rgba.FCurve->G.GetOffset(*(manager));
		value.color.fcurve_rgba.offset[2] = param.fcurve_rgba.FCurve->B.GetOffset(*(manager));
		value.color.fcurve_rgba.offset[3] = param.fcurve_rgba.FCurve->A.GetOffset(*(manager));
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeValues(InstanceGroupValues::Size& value, TrackSizeParameter& param, Manager* manager)
{
	if( param.type == TrackSizeParameter::Fixed )
	{
		value.size.fixed.size_ = param.fixed.size;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::SetValues( Color& c, InstanceGroupValues::Color& value, StandardColorParameter& param, int32_t time, int32_t livedTime )
{
	if( param.type == StandardColorParameter::Fixed )
	{
		value.color.fixed.color_.setValueToArg( c );
	}
	else if(param.type == StandardColorParameter::Random )
	{
		value.color.random.color_.setValueToArg( c );
	}
	else if( param.type == StandardColorParameter::Easing )
	{
		float t = (float)time / (float)livedTime;
		param.easing.all.setValueToArg(
			c, 
			value.color.easing.start,
			value.color.easing.end,
			t );
	}
	else if( param.type == StandardColorParameter::FCurve_RGBA )
	{
		c.R = (uint8_t)Clamp( (value.color.fcurve_rgba.offset[0] + param.fcurve_rgba.FCurve->R.GetValue( (int32_t)time )), 255, 0);
		c.G = (uint8_t)Clamp( (value.color.fcurve_rgba.offset[1] + param.fcurve_rgba.FCurve->G.GetValue( (int32_t)time )), 255, 0);
		c.B = (uint8_t)Clamp( (value.color.fcurve_rgba.offset[2] + param.fcurve_rgba.FCurve->B.GetValue( (int32_t)time )), 255, 0);
		c.A = (uint8_t)Clamp( (value.color.fcurve_rgba.offset[3] + param.fcurve_rgba.FCurve->A.GetValue( (int32_t)time )), 255, 0);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::SetValues( float& s, InstanceGroupValues::Size& value, TrackSizeParameter& param, float time )
{
	if( param.type == TrackSizeParameter::Fixed )
	{
		s = value.size.fixed.size_;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::LoadValues( TrackSizeParameter& param, unsigned char*& pos )
{
	memcpy( &param.type, pos, sizeof(int) );
	pos += sizeof(int);
	
	if( param.type == TrackSizeParameter::Fixed )
	{	
		memcpy( &param.fixed, pos, sizeof(param.fixed) );
		pos += sizeof(param.fixed);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

static void PathCombine(EFK_CHAR* dst, const EFK_CHAR* src1, const EFK_CHAR* src2)
{
	int len1 = 0, len2 = 0;
	if( src1 != NULL )
	{
		for( len1 = 0; src1[len1] != L'\0'; len1++ ) {}
		memcpy( dst, src1, len1 * sizeof(EFK_CHAR) );
		if( len1 > 0 && src1[len1 - 1] != L'/' && src1[len1 - 1] != L'\\' )
		{
			dst[len1++] = L'/';
		}
	}
	if( src2 != NULL)
	{
		for( len2 = 0; src2[len2] != L'\0'; len2++ ) {}
		memcpy( &dst[len1], src2, len2 * sizeof(EFK_CHAR) );
	}
	dst[len1 + len2] = L'\0';
}

static void GetParentDir(EFK_CHAR* dst, const EFK_CHAR* src)
{
	int i, last = -1;
	for( i = 0; src[i] != L'\0'; i++ )
	{
		if( src[i] == L'/' || src[i] == L'\\' )
			last = i;
	}
	if( last >= 0 )
	{
		memcpy( dst, src, last * sizeof(EFK_CHAR) );
		dst[last] = L'\0';
	}
	else
	{
		dst[0] = L'\0';
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Effect::Create( Manager* manager, void* data, int32_t size, float magnification, const EFK_CHAR* materialPath )
{
	return EffectImplemented::Create( manager, data, size, magnification, materialPath );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Effect::Create(Manager* manager, const EFK_CHAR* path, float magnification, const EFK_CHAR* materialPath)
{
	Setting* setting = manager->GetSetting();

	EffectLoader* eLoader = setting->GetEffectLoader();

	if (setting == NULL) return NULL;

	void* data = NULL;
	int32_t size = 0;

	if (!eLoader->Load(path, data, size)) return NULL;

	EFK_CHAR parentDir[512];
	if (materialPath == NULL)
	{
		GetParentDir(parentDir, path);
		materialPath = parentDir;
	}

	Effect* effect = EffectImplemented::Create(manager, data, size, magnification, materialPath);

	eLoader->Unload(data, size);

	return effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* EffectImplemented::Create( Manager* pManager, void* pData, int size, float magnification, const EFK_CHAR* materialPath )
{
	if( pData == NULL || size == 0 ) return NULL;

	EffectImplemented* effect = new EffectImplemented( pManager, pData, size );
	if ( !effect->Load( pData, size, magnification, materialPath ) )
	{
		effect->Release();
		effect = NULL;
	}
	return effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Effect::Create( Setting* setting, void* data, int32_t size, float magnification, const EFK_CHAR* materialPath )
{
	return EffectImplemented::Create(setting, data, size, magnification, materialPath );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Effect::Create( Setting* setting, const EFK_CHAR* path, float magnification, const EFK_CHAR* materialPath)
{
	if(setting == NULL) return NULL;
	EffectLoader* eLoader = setting->GetEffectLoader();

	if (setting == NULL) return NULL;

	void* data = NULL;
	int32_t size = 0;

	if (!eLoader->Load(path, data, size)) return NULL;

	EFK_CHAR parentDir[512];
	if (materialPath == NULL)
	{
		GetParentDir(parentDir, path);
		materialPath = parentDir;
	}

	Effect* effect = EffectImplemented::Create(setting, data, size, magnification, materialPath);

	eLoader->Unload(data, size);

	return effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* EffectImplemented::Create( Setting* setting, void* pData, int size, float magnification, const EFK_CHAR* materialPath )
{
	if( pData == NULL || size == 0 ) return NULL;

	EffectImplemented* effect = new EffectImplemented( setting, pData, size );
	if ( !effect->Load( pData, size, magnification, materialPath ) )
	{
		effect->Release();
		effect = NULL;
	}
	return effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::EffectLoader* Effect::CreateEffectLoader(::Effekseer::FileInterface* fileInterface)
{
	return new ::Effekseer::DefaultEffectLoader(fileInterface);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectImplemented::EffectImplemented( Manager* pManager, void* pData, int size )
	: m_pManager		( (ManagerImplemented*)pManager )
	, m_setting			(NULL)
	, m_reference		( 1 )
	, m_version			( 0 )
	, m_ImageCount		( 0 )
	, m_ImagePaths		( NULL )
	, m_pImages			( NULL )
	, m_WaveCount		( 0 )
	, m_WavePaths		( NULL )
	, m_pWaves			( NULL )
	, m_modelCount		( 0 )
	, m_modelPaths		( NULL )
	, m_pModels			( NULL )
	, m_maginification	( 1.0f )
	, m_maginificationExternal	( 1.0f )
	, m_pRoot			( NULL )

	, m_normalImageCount(0)
	, m_normalImagePaths(nullptr)
	, m_normalImages(nullptr)

	, m_distortionImageCount(0)
	, m_distortionImagePaths(nullptr)
	, m_distortionImages(nullptr)

{
	ES_SAFE_ADDREF( m_pManager );

	Culling.Shape = CullingShape::NoneShape;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectImplemented::EffectImplemented( Setting* setting, void* pData, int size )
	: m_pManager		( NULL )
	, m_setting			(setting)
	, m_reference		( 1 )
	, m_version			( 0 )
	, m_ImageCount		( 0 )
	, m_ImagePaths		( NULL )
	, m_pImages			( NULL )
	, m_WaveCount		( 0 )
	, m_WavePaths		( NULL )
	, m_pWaves			( NULL )
	, m_modelCount		( 0 )
	, m_modelPaths		( NULL )
	, m_pModels			( NULL )
	, m_maginification	( 1.0f )
	, m_maginificationExternal	( 1.0f )
	, m_pRoot			( NULL )

	, m_normalImageCount(0)
	, m_normalImagePaths(nullptr)
	, m_normalImages(nullptr)

	, m_distortionImageCount(0)
	, m_distortionImagePaths(nullptr)
	, m_distortionImages(nullptr)
{
	ES_SAFE_ADDREF( m_setting );
	
	Culling.Shape = CullingShape::NoneShape;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectImplemented::~EffectImplemented()
{
	Reset();

	ES_SAFE_RELEASE( m_setting );
	ES_SAFE_RELEASE( m_pManager );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNode* EffectImplemented::GetRoot() const
{
	return m_pRoot;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float EffectImplemented::GetMaginification() const
{
	return m_maginification;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Load( void* pData, int size, float mag, const EFK_CHAR* materialPath )
{
	EffekseerPrintDebug("** Create : Effect\n");

	uint8_t* pos = (uint8_t*)pData;

	// EFKS
	int head = 0;
	memcpy( &head, pos, sizeof(int) );
	if( memcmp( &head, "SKFE", 4 ) != 0 ) return false;
	pos += sizeof( int );

	memcpy( &m_version, pos, sizeof(int) );
	pos += sizeof(int);

	// 画像
	memcpy( &m_ImageCount, pos, sizeof(int) );
	pos += sizeof(int);

	if( m_ImageCount > 0 )
	{
		m_ImagePaths = new EFK_CHAR*[ m_ImageCount ];
		m_pImages = new void*[ m_ImageCount ];

		for( int i = 0; i < m_ImageCount; i++ )
		{
			int length = 0;
			memcpy( &length, pos, sizeof(int) );
			pos += sizeof(int);

			m_ImagePaths[i] = new EFK_CHAR[ length ];
			memcpy( m_ImagePaths[i], pos, length * sizeof(EFK_CHAR) );
			pos += length * sizeof(EFK_CHAR);

			m_pImages[i] = NULL;
		}
	}

	if (m_version >= 9)
	{
		// 画像
		memcpy(&m_normalImageCount, pos, sizeof(int));
		pos += sizeof(int);

		if (m_normalImageCount > 0)
		{
			m_normalImagePaths = new EFK_CHAR*[m_normalImageCount];
			m_normalImages = new void*[m_normalImageCount];

			for (int i = 0; i < m_normalImageCount; i++)
			{
				int length = 0;
				memcpy(&length, pos, sizeof(int));
				pos += sizeof(int);

				m_normalImagePaths[i] = new EFK_CHAR[length];
				memcpy(m_normalImagePaths[i], pos, length * sizeof(EFK_CHAR));
				pos += length * sizeof(EFK_CHAR);

				m_normalImages[i] = NULL;
			}
		}

		// 画像
		memcpy(&m_distortionImageCount, pos, sizeof(int));
		pos += sizeof(int);

		if (m_distortionImageCount > 0)
		{
			m_distortionImagePaths = new EFK_CHAR*[m_distortionImageCount];
			m_distortionImages = new void*[m_distortionImageCount];

			for (int i = 0; i < m_distortionImageCount; i++)
			{
				int length = 0;
				memcpy(&length, pos, sizeof(int));
				pos += sizeof(int);

				m_distortionImagePaths[i] = new EFK_CHAR[length];
				memcpy(m_distortionImagePaths[i], pos, length * sizeof(EFK_CHAR));
				pos += length * sizeof(EFK_CHAR);

				m_distortionImages[i] = NULL;
			}
		}
	}

	if( m_version >= 1 )
	{
		// ウェーブ
		memcpy( &m_WaveCount, pos, sizeof(int) );
		pos += sizeof(int);

		if( m_WaveCount > 0 )
		{
			m_WavePaths = new EFK_CHAR*[ m_WaveCount ];
			m_pWaves = new void*[ m_WaveCount ];

			for( int i = 0; i < m_WaveCount; i++ )
			{
				int length = 0;
				memcpy( &length, pos, sizeof(int) );
				pos += sizeof(int);

				m_WavePaths[i] = new EFK_CHAR[ length ];
				memcpy( m_WavePaths[i], pos, length * sizeof(EFK_CHAR) );
				pos += length * sizeof(EFK_CHAR);

				m_pWaves[i] = NULL;
			}
		}
	}

	if( m_version >= 6 )
	{
		/* モデル */
		memcpy( &m_modelCount, pos, sizeof(int) );
		pos += sizeof(int);

		if( m_modelCount > 0 )
		{
			m_modelPaths = new EFK_CHAR*[ m_modelCount ];
			m_pModels = new void*[ m_modelCount ];

			for( int i = 0; i < m_modelCount; i++ )
			{
				int length = 0;
				memcpy( &length, pos, sizeof(int) );
				pos += sizeof(int);

				m_modelPaths[i] = new EFK_CHAR[ length ];
				memcpy( m_modelPaths[i], pos, length * sizeof(EFK_CHAR) );
				pos += length * sizeof(EFK_CHAR);

				m_pModels[i] = NULL;
			}
		}
	}

	// 拡大率
	if( m_version >= 2 )
	{
		memcpy( &m_maginification, pos, sizeof(float) );
		pos += sizeof(float);
		m_maginification *= mag;
		m_maginificationExternal = mag;
	}

	// カリング
	if( m_version >= 9 )
	{
		memcpy( &(Culling.Shape), pos, sizeof(int32_t) );
		pos += sizeof(int32_t);
		if(Culling.Shape ==	CullingShape::Sphere)
		{
			memcpy( &(Culling.Sphere.Radius), pos, sizeof(float) );
			pos += sizeof(float);
		
			memcpy( &(Culling.Location.X), pos, sizeof(float) );
			pos += sizeof(float);
			memcpy( &(Culling.Location.Y), pos, sizeof(float) );
			pos += sizeof(float);
			memcpy( &(Culling.Location.Z), pos, sizeof(float) );
			pos += sizeof(float);
		}
	}

	// ノード
	m_pRoot = EffectNodeImplemented::Create( this, NULL, pos );

	// リロード用にmaterialPathを記録しておく
    if (materialPath) m_materialPath = materialPath;

	ReloadResources( materialPath );
	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectImplemented::Reset()
{
	UnloadResources();

	Setting* loader = GetSetting();

	TextureLoader* textureLoader = loader->GetTextureLoader();

	for( int i = 0; i < m_ImageCount; i++ )
	{
		if( m_ImagePaths[i] != NULL ) delete [] m_ImagePaths[i];
	}

	m_ImageCount = 0;

	ES_SAFE_DELETE_ARRAY( m_ImagePaths );
	ES_SAFE_DELETE_ARRAY(m_pImages);

	{
		for (int i = 0; i < m_normalImageCount; i++)
		{
			if (m_normalImagePaths[i] != NULL) delete [] m_normalImagePaths[i];
		}

		m_normalImageCount = 0;

		ES_SAFE_DELETE_ARRAY(m_normalImagePaths);
		ES_SAFE_DELETE_ARRAY(m_normalImages);
	}

	{
		for (int i = 0; i < m_distortionImageCount; i++)
		{
			if (m_distortionImagePaths[i] != NULL) delete [] m_distortionImagePaths[i];
		}

		m_distortionImageCount = 0;

		ES_SAFE_DELETE_ARRAY(m_distortionImagePaths);
		ES_SAFE_DELETE_ARRAY(m_distortionImages);
	}

	for( int i = 0; i < m_WaveCount; i++ )
	{
		if( m_WavePaths[i] != NULL ) delete [] m_WavePaths[i];
	}
	m_WaveCount = 0;

	ES_SAFE_DELETE_ARRAY( m_WavePaths );
	ES_SAFE_DELETE_ARRAY( m_pWaves );

	for( int i = 0; i < m_modelCount; i++ )
	{
		if( m_modelPaths[i] != NULL ) delete [] m_modelPaths[i];
	}
	m_modelCount = 0;

	ES_SAFE_DELETE_ARRAY( m_modelPaths );
	ES_SAFE_DELETE_ARRAY( m_pModels );

	ES_SAFE_DELETE( m_pRoot );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Manager* EffectImplemented::GetManager() const
{
	return m_pManager;	
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Setting* EffectImplemented::GetSetting() const
{
	if(m_setting != NULL) return m_setting;
	return m_pManager->GetSetting();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int EffectImplemented::GetVersion() const
{
	return m_version;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* EffectImplemented::GetColorImage( int n ) const
{
	return m_pImages[ n ];
}

int32_t EffectImplemented::GetColorImageCount() const
{
	return m_ImageCount;
}

void* EffectImplemented::GetNormalImage(int n) const
{
	/* 強制的に互換をとる */
	if (this->m_version <= 8)
	{
		return m_pImages[n];
	}

	return m_normalImages[n];
}

int32_t EffectImplemented::GetNormalImageCount() const
{
	return m_normalImageCount;
}

void* EffectImplemented::GetDistortionImage(int n) const
{
	/* 強制的に互換をとる */
	if (this->m_version <= 8)
	{
		return m_pImages[n];
	}

	return m_distortionImages[n];
}

int32_t EffectImplemented::GetDistortionImageCount() const
{
	return m_distortionImageCount;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* EffectImplemented::GetWave( int n ) const
{
	return m_pWaves[ n ];
}

int32_t EffectImplemented::GetWaveCount() const
{
	return m_WaveCount;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* EffectImplemented::GetModel( int n ) const
{
	return m_pModels[ n ];
}

int32_t EffectImplemented::GetModelCount() const
{
	return m_modelCount;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( void* data, int32_t size, const EFK_CHAR* materialPath )
{
	if(m_pManager == NULL ) return false;

	return Reload( m_pManager, 1, data, size, materialPath );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( const EFK_CHAR* path, const EFK_CHAR* materialPath )
{
	if(m_pManager == NULL ) return false;

	return Reload( m_pManager, 1, path, materialPath );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( Manager* managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath )
{
	if(m_pManager == NULL ) return false;

	const EFK_CHAR* matPath = materialPath != NULL ? materialPath : m_materialPath.c_str();
	
	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)&(managers[i]))->BeginReloadEffect( this );
	}

	Reset();
	Load( data, size, m_maginificationExternal, matPath );

	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)&(managers[i]))->EndReloadEffect( this );
	}

	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( Manager* managers, int32_t managersCount, const EFK_CHAR* path, const EFK_CHAR* materialPath )
{
	if(m_pManager == NULL ) return false;

	Setting* loader = GetSetting();
	
	EffectLoader* eLoader = loader->GetEffectLoader();
	if( loader == NULL ) return false;

	void* data = NULL;
	int32_t size = 0;

	if( !eLoader->Load( path, data, size ) ) return false;

	EFK_CHAR parentDir[512];
	if( materialPath == NULL )
	{
		GetParentDir(parentDir, path);
		materialPath = parentDir;
	}

	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)&(managers[i]))->BeginReloadEffect( this );
	}

	Reset();
	Load( data, size, m_maginificationExternal, materialPath );

	m_pManager->EndReloadEffect( this );

	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)&(managers[i]))->EndReloadEffect( this );
	}

	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectImplemented::ReloadResources( const EFK_CHAR* materialPath )
{
	UnloadResources();

	const EFK_CHAR* matPath = materialPath != NULL ? materialPath : m_materialPath.c_str();
	
	Setting* loader = GetSetting();

	{
		TextureLoader* textureLoader = loader->GetTextureLoader();
		if( textureLoader != NULL )
		{
			for( int32_t ind = 0; ind < m_ImageCount; ind++ )
			{
				EFK_CHAR fullPath[512];
				PathCombine( fullPath, matPath, m_ImagePaths[ ind ] );
				m_pImages[ind] = textureLoader->Load( fullPath, TextureType::Color );
			}
		}
	}

	{
		TextureLoader* textureLoader = loader->GetTextureLoader();
		if (textureLoader != NULL)
		{
			for (int32_t ind = 0; ind < m_normalImageCount; ind++)
			{
				EFK_CHAR fullPath[512];
				PathCombine(fullPath, matPath, m_normalImagePaths[ind]);
				m_normalImages[ind] = textureLoader->Load(fullPath, TextureType::Normal);
			}
		}

	}
		{
			TextureLoader* textureLoader = loader->GetTextureLoader();
			if (textureLoader != NULL)
			{
				for (int32_t ind = 0; ind < m_distortionImageCount; ind++)
				{
					EFK_CHAR fullPath[512];
					PathCombine(fullPath, matPath, m_distortionImagePaths[ind]);
					m_distortionImages[ind] = textureLoader->Load(fullPath, TextureType::Distortion);
				}
			}
		}

	

	{
		SoundLoader* soundLoader = loader->GetSoundLoader();
		if( soundLoader != NULL )
		{
			for( int32_t ind = 0; ind < m_WaveCount; ind++ )
			{
				EFK_CHAR fullPath[512];
				PathCombine( fullPath, matPath, m_WavePaths[ ind ] );
				m_pWaves[ind] = soundLoader->Load( fullPath );
			}
		}
	}

	{
		ModelLoader* modelLoader = loader->GetModelLoader();
		
		if( modelLoader != NULL )
		{
			for( int32_t ind = 0; ind < m_modelCount; ind++ )
			{
				EFK_CHAR fullPath[512];
				PathCombine( fullPath, matPath, m_modelPaths[ ind ] );
				m_pModels[ind] = modelLoader->Load( fullPath );
			}
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectImplemented::UnloadResources()
{
	Setting* loader = GetSetting();
	
	TextureLoader* textureLoader = loader->GetTextureLoader();
	if( textureLoader != NULL )
	{
		for( int32_t ind = 0; ind < m_ImageCount; ind++ )
		{
			textureLoader->Unload( m_pImages[ind] );
			m_pImages[ind] = NULL;
		}

		for (int32_t ind = 0; ind < m_normalImageCount; ind++)
		{
			textureLoader->Unload(m_normalImages[ind]);
			m_normalImages[ind] = NULL;
		}

		for (int32_t ind = 0; ind < m_distortionImageCount; ind++)
		{
			textureLoader->Unload(m_distortionImages[ind]);
			m_distortionImages[ind] = NULL;
		}
	}

	SoundLoader* soundLoader = loader->GetSoundLoader();
	if( soundLoader != NULL )
	{
		for( int32_t ind = 0; ind < m_WaveCount; ind++ )
		{
			soundLoader->Unload( m_pWaves[ind] );
			m_pWaves[ind] = NULL;
		}
	}

	{
		ModelLoader* modelLoader = loader->GetModelLoader();
		if( modelLoader != NULL )
		{
			for( int32_t ind = 0; ind < m_modelCount; ind++ )
			{
				modelLoader->Unload( m_pModels[ind] );
				m_pModels[ind] = NULL;
			}
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------







#ifdef _WIN32
#else
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static int64_t GetTime(void)
{
#ifdef _WIN32
	int64_t count, freq;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&count))
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
		{
			return count * 1000000 / freq;
		}
	}
	return 0;
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Manager* Manager::Create( int instance_max, bool autoFlip )
{
	return new ManagerImplemented( instance_max, autoFlip );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Handle ManagerImplemented::AddDrawSet( Effect* effect, InstanceContainer* pInstanceContainer, InstanceGlobal* pGlobalPointer )
{
	Handle Temp = m_NextHandle;

	if( ++m_NextHandle < 0 )
	{
		// オーバーフローしたらリセットする
		m_NextHandle = 0;
	}

	DrawSet drawset( effect, pInstanceContainer, pGlobalPointer );
	drawset.Self = Temp;

	ES_SAFE_ADDREF( effect );

	m_DrawSets[Temp] = drawset;

	return Temp;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::GCDrawSet( bool isRemovingManager )
{
	// インスタンスグループ自体の削除処理
	{
		std::map<Handle,DrawSet>::iterator it = m_RemovingDrawSets[1].begin();
		while( it != m_RemovingDrawSets[1].end() )
		{
			DrawSet& drawset = (*it).second;

			// 全破棄処理
			drawset.InstanceContainerPointer->RemoveForcibly( true );
			drawset.InstanceContainerPointer->~InstanceContainer();
			InstanceContainer::operator delete( drawset.InstanceContainerPointer, this );
			ES_SAFE_RELEASE( drawset.ParameterPointer );
			ES_SAFE_DELETE( drawset.GlobalPointer );

			if(m_cullingWorld != NULL)
			{
				m_cullingWorld->RemoveObject(drawset.CullingObjectPointer);
				Culling3D::SafeRelease(drawset.CullingObjectPointer);
			}

			m_RemovingDrawSets[1].erase( it++ );
		}
		m_RemovingDrawSets[1].clear();
	}

	{
		std::map<Handle,DrawSet>::iterator it = m_RemovingDrawSets[0].begin();
		while( it != m_RemovingDrawSets[0].end() )
		{
			DrawSet& drawset = (*it).second;
			m_RemovingDrawSets[1][ (*it).first ] = (*it).second;
			m_RemovingDrawSets[0].erase( it++ );
		}
		m_RemovingDrawSets[0].clear();
	}

	{
		std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
		while( it != m_DrawSets.end() )
		{
			DrawSet& draw_set = (*it).second;

			// 削除フラグが立っている時
			bool isRemoving = draw_set.IsRemoving;

			// 何も存在しない時
			if( !isRemoving && draw_set.GlobalPointer->GetInstanceCount() == 0 )
			{
				isRemoving = true;
			}

			// ルートのみ存在し、既に新しく生成する見込みがないとき
			if( !isRemoving && draw_set.GlobalPointer->GetInstanceCount() == 1 )
			{
				InstanceContainer* pRootContainer = draw_set.InstanceContainerPointer;
				InstanceGroup* group = pRootContainer->GetFirstGroup();

				if( group )
				{
					Instance* pRootInstance = group->GetFirst();

					if( pRootInstance && pRootInstance->GetState() == INSTANCE_STATE_ACTIVE )
					{
						int maxcreate_count = 0;
						for( int i = 0; i < Min(pRootInstance->m_pEffectNode->GetChildrenCount(), Instance::ChildrenMax); i++ )
						{
							auto child = (EffectNodeImplemented*) pRootInstance->m_pEffectNode->GetChild(i);

							float last_generation_time = 
								child->CommonValues.GenerationTime.max *
								(child->CommonValues.MaxGeneration - 1) +
								child->CommonValues.GenerationTimeOffset.max +
								1.0f;

							if( pRootInstance->m_LivingTime >= last_generation_time )
							{
								maxcreate_count++;
							}
							else
							{
								break;
							}
						}
					
						if( maxcreate_count == pRootInstance->m_pEffectNode->GetChildrenCount() )
						{
							// 音が再生中でないとき
							if (!GetSoundPlayer() || !GetSoundPlayer()->CheckPlayingTag(draw_set.GlobalPointer))
							{
								isRemoving = true;
							}
						}
					}
				}
			}

			if( isRemoving )
			{
				// 消去処理
				StopEffect( (*it).first );

				if( (*it).second.RemovingCallback != NULL )
				{
					(*it).second.RemovingCallback( this, (*it).first, isRemovingManager );
				}

				m_RemovingDrawSets[0][ (*it).first ] = (*it).second;
				m_DrawSets.erase( it++ );
			}
			else
			{
				++it;
			}			
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer* ManagerImplemented::CreateInstanceContainer( EffectNode* pEffectNode, InstanceGlobal* pGlobal, bool isRoot, Instance* pParent )
{
	InstanceContainer* pContainer = new(this) InstanceContainer(
		this,
		pEffectNode,
		pGlobal,
		pEffectNode->GetChildrenCount() );
	
	for( int i = 0; i < pEffectNode->GetChildrenCount(); i++ )
	{
		pContainer->SetChild( i, CreateInstanceContainer( pEffectNode->GetChild(i), pGlobal ) );
	}

	if( isRoot )
	{
		InstanceGroup* group = pContainer->CreateGroup();
		Instance* instance = group->CreateInstance();
		instance->Initialize( NULL, 0 );

		/* インスタンスが生成したわけではないためfalseに変更 */
		group->IsReferencedFromInstance = false;
	}

	return pContainer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* EFK_STDCALL ManagerImplemented::Malloc( unsigned int size )
{
	return (void*)new char*[size];
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EFK_STDCALL ManagerImplemented::Free( void* p, unsigned int size )
{
	char* pData = (char*)p;
	delete [] pData;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int EFK_STDCALL ManagerImplemented::Rand()
{
	return rand();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::ExecuteEvents()
{	
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();

	while( it != it_end )
	{
		if( (*it).second.GoingToStop )
		{
			InstanceContainer* pContainer = (*it).second.InstanceContainerPointer;
			pContainer->KillAllInstances( true );
			(*it).second.IsRemoving = true;
			if (GetSoundPlayer() != NULL)
			{
				GetSoundPlayer()->StopTag((*it).second.GlobalPointer);
			}
		}

		if( (*it).second.GoingToStopRoot )
		{
			InstanceContainer* pContainer = (*it).second.InstanceContainerPointer;
			pContainer->KillAllInstances( false );
		}

		++it;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ManagerImplemented::ManagerImplemented( int instance_max, bool autoFlip )
	: m_autoFlip	( autoFlip )
	, m_NextHandle	( 0 )
	, m_instance_max	( instance_max )
	, m_setting			( NULL )
	, m_sequenceNumber	( 0 )

	, m_cullingWorld	(NULL)
	, m_culled(false)

	, m_MallocFunc(NULL)
	, m_FreeFunc(NULL)
	, m_randFunc(NULL)
	, m_randMax(0)

	, m_spriteRenderer(NULL)
	, m_ribbonRenderer(NULL)
	, m_ringRenderer(NULL)
	, m_modelRenderer(NULL)
	, m_trackRenderer(NULL)

	, m_soundPlayer(NULL)
{
	m_setting = Setting::Create();

	SetMallocFunc( Malloc );
	SetFreeFunc( Free );
	SetRandFunc( Rand );
	SetRandMax( RAND_MAX );

	m_renderingDrawSets.reserve( 64 );

	m_reserved_instances_buffer = new uint8_t[ sizeof(Instance) * m_instance_max ];

	for( int i = 0; i < m_instance_max; i++ )
	{
		Instance* instances = (Instance*)m_reserved_instances_buffer;
		m_reserved_instances.push( &instances[i] );
	}

	m_setting->SetEffectLoader(new DefaultEffectLoader());
	EffekseerPrintDebug("*** Create : Manager\n");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ManagerImplemented::~ManagerImplemented()
{
	StopAllEffects();

	ExecuteEvents();

	for( int i = 0; i < 5; i++ )
	{
		GCDrawSet( true );
	}

	assert( m_reserved_instances.size() == m_instance_max ); 
	ES_SAFE_DELETE_ARRAY( m_reserved_instances_buffer );

	Culling3D::SafeRelease(m_cullingWorld);

	ES_SAFE_DELETE(m_spriteRenderer);
	ES_SAFE_DELETE(m_ribbonRenderer);
	ES_SAFE_DELETE(m_modelRenderer);
	ES_SAFE_DELETE(m_trackRenderer);
	ES_SAFE_DELETE(m_ringRenderer);

	ES_SAFE_DELETE(m_soundPlayer);

	ES_SAFE_RELEASE( m_setting );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::PushInstance( Instance* instance )
{
	m_reserved_instances.push( instance );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Instance* ManagerImplemented::PopInstance()
{
	if( m_reserved_instances.empty() )
	{
		return NULL;
	}

	Instance* ret = m_reserved_instances.front();
	m_reserved_instances.pop();
	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Destroy()
{
	StopAllEffects();

	ExecuteEvents();

	for( int i = 0; i < 5; i++ )
	{
		GCDrawSet( true );
	}

	Release();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
uint32_t ManagerImplemented::GetSequenceNumber() const
{
	return m_sequenceNumber;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
MallocFunc ManagerImplemented::GetMallocFunc() const
{
	return m_MallocFunc;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetMallocFunc(MallocFunc func)
{
	m_MallocFunc = func;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
FreeFunc ManagerImplemented::GetFreeFunc() const
{
	return m_FreeFunc;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetFreeFunc(FreeFunc func)
{
	m_FreeFunc = func;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RandFunc ManagerImplemented::GetRandFunc() const
{
	return m_randFunc;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRandFunc(RandFunc func)
{
	m_randFunc = func;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int ManagerImplemented::GetRandMax() const
{
	return m_randMax;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRandMax(int max_)
{
	m_randMax = max_;
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
CoordinateSystem ManagerImplemented::GetCoordinateSystem() const
{
	return m_setting->GetCoordinateSystem();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetCoordinateSystem( CoordinateSystem coordinateSystem )
{
	m_setting->SetCoordinateSystem(coordinateSystem);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SpriteRenderer* ManagerImplemented::GetSpriteRenderer()
{
	return m_spriteRenderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetSpriteRenderer(SpriteRenderer* renderer)
{
	ES_SAFE_DELETE(m_spriteRenderer);
	m_spriteRenderer = renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RibbonRenderer* ManagerImplemented::GetRibbonRenderer()
{
	return m_ribbonRenderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRibbonRenderer(RibbonRenderer* renderer)
{
	ES_SAFE_DELETE(m_ribbonRenderer);
	m_ribbonRenderer = renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RingRenderer* ManagerImplemented::GetRingRenderer()
{
	return m_ringRenderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRingRenderer(RingRenderer* renderer)
{
	ES_SAFE_DELETE(m_ringRenderer);
	m_ringRenderer = renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer* ManagerImplemented::GetModelRenderer()
{
	return m_modelRenderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetModelRenderer(ModelRenderer* renderer)
{
	ES_SAFE_DELETE(m_modelRenderer);
	m_modelRenderer = renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
TrackRenderer* ManagerImplemented::GetTrackRenderer()
{
	return m_trackRenderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetTrackRenderer(TrackRenderer* renderer)
{
	ES_SAFE_DELETE(m_trackRenderer);
	m_trackRenderer = renderer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundPlayer* ManagerImplemented::GetSoundPlayer()
{
	return m_soundPlayer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetSoundPlayer(SoundPlayer* soundPlayer)
{
	ES_SAFE_DELETE(m_soundPlayer);
	m_soundPlayer = soundPlayer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Setting* ManagerImplemented::GetSetting()
{
	return m_setting;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetSetting(Setting* setting)
{
	ES_SAFE_RELEASE(m_setting);
	m_setting = setting;
	ES_SAFE_ADDREF(m_setting);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectLoader* ManagerImplemented::GetEffectLoader()
{
	return m_setting->GetEffectLoader();
}
	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetEffectLoader( EffectLoader* effectLoader )
{
	m_setting->SetEffectLoader(effectLoader);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
TextureLoader* ManagerImplemented::GetTextureLoader()
{
	return m_setting->GetTextureLoader();
}
	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetTextureLoader( TextureLoader* textureLoader )
{
	m_setting->SetTextureLoader(textureLoader);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundLoader* ManagerImplemented::GetSoundLoader()
{
	return m_setting->GetSoundLoader();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetSoundLoader( SoundLoader* soundLoader )
{
	m_setting->SetSoundLoader(soundLoader);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelLoader* ManagerImplemented::GetModelLoader()
{
	return m_setting->GetModelLoader();
}
	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetModelLoader( ModelLoader* modelLoader )
{
	m_setting->SetModelLoader(modelLoader);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::StopEffect( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];
		drawSet.GoingToStop = true;
		drawSet.IsRemoving = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::StopAllEffects()
{
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	while( it != m_DrawSets.end() )
	{
		(*it).second.GoingToStop = true;
		(*it).second.IsRemoving = true;
		++it;		
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::StopRoot( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].GoingToStopRoot = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::StopRoot( Effect* effect )
{
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();

	while( it != it_end )
	{
		if( (*it).second.ParameterPointer == effect )
		{
			(*it).second.GoingToStopRoot = true;
		}
		++it;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ManagerImplemented::Exists( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		if( m_DrawSets[handle].IsRemoving ) return false;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t ManagerImplemented::GetInstanceCount( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		return m_DrawSets[handle].GlobalPointer->GetInstanceCount();
	}
	return 0;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix43 ManagerImplemented::GetMatrix( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		InstanceGroup* pGroup = pContainer->GetFirstGroup();

		if( pGroup != NULL )
		{
			Instance* instance = pGroup->GetFirst();
			if( instance != NULL )
			{
				return instance->m_GlobalMatrix43;
			}
		}

		return Matrix43();
	}

	return Matrix43();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetMatrix( Handle handle, const Matrix43& mat )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		InstanceGroup* pGroup = pContainer->GetFirstGroup();

		if( pGroup != NULL )
		{
			Instance* instance = pGroup->GetFirst();
			if( instance != NULL )
			{
				instance->m_GlobalMatrix43 = mat;

				drawSet.GlobalMatrix = instance->m_GlobalMatrix43;
				drawSet.IsParameterChanged = true;
			}
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Vector3D ManagerImplemented::GetLocation( Handle handle )
{
	Vector3D location;

	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

		location.X = pInstance->m_GlobalMatrix43.Value[3][0];
		location.Y = pInstance->m_GlobalMatrix43.Value[3][1];
		location.Z = pInstance->m_GlobalMatrix43.Value[3][2];
	}

	return location;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetLocation( Handle handle, float x, float y, float z )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

		pInstance->m_GlobalMatrix43.Value[3][0] = x;
		pInstance->m_GlobalMatrix43.Value[3][1] = y;
		pInstance->m_GlobalMatrix43.Value[3][2] = z;

		drawSet.GlobalMatrix = pInstance->m_GlobalMatrix43;
		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetLocation( Handle handle, const Vector3D& location )
{
	SetLocation( handle, location.X, location.Y, location.Z );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::AddLocation( Handle handle, const Vector3D& location )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

		pInstance->m_GlobalMatrix43.Value[3][0] += location.X;
		pInstance->m_GlobalMatrix43.Value[3][1] += location.Y;
		pInstance->m_GlobalMatrix43.Value[3][2] += location.Z;

		drawSet.GlobalMatrix = pInstance->m_GlobalMatrix43;
		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRotation( Handle handle, float x, float y, float z )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;

		if( pContainer == NULL ) return;
		if( pContainer->GetFirstGroup() == NULL ) return;
		if( pContainer->GetFirstGroup()->GetFirst() == NULL ) return;

		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();
		
		Matrix43 MatRotX,MatRotY,MatRotZ;

		MatRotX.RotationX( x );
		MatRotY.RotationY( y );
		MatRotZ.RotationZ( z );
		
		Matrix43 r;
		Vector3D s, t;

		pInstance->m_GlobalMatrix43.GetSRT( s, r, t );

		r.Indentity();
		Matrix43::Multiple( r, r, MatRotZ );
		Matrix43::Multiple( r, r, MatRotX );
		Matrix43::Multiple( r, r, MatRotY );

		pInstance->m_GlobalMatrix43.SetSRT( s, r, t );
		
		drawSet.GlobalMatrix = pInstance->m_GlobalMatrix43;
		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRotation( Handle handle, const Vector3D& axis, float angle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		if( pContainer == NULL ) return;
		if( pContainer->GetFirstGroup() == NULL ) return;
		if( pContainer->GetFirstGroup()->GetFirst() == NULL ) return;

		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

		Matrix43 r;
		Vector3D s, t;

		pInstance->m_GlobalMatrix43.GetSRT( s, r, t );

		r.RotationAxis( axis, angle );

		pInstance->m_GlobalMatrix43.SetSRT( s, r, t );
		
		drawSet.GlobalMatrix = pInstance->m_GlobalMatrix43;
		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetScale( Handle handle, float x, float y, float z )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		if( pContainer == NULL ) return;
		if( pContainer->GetFirstGroup() == NULL ) return;
		if( pContainer->GetFirstGroup()->GetFirst() == NULL ) return;

		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

		Matrix43 r;
		Vector3D s, t;

		pInstance->m_GlobalMatrix43.GetSRT( s, r, t );

		s.X = x;
		s.Y = y;
		s.Z = z;

		pInstance->m_GlobalMatrix43.SetSRT( s, r, t );

		drawSet.GlobalMatrix = pInstance->m_GlobalMatrix43;
		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetTargetLocation( Handle handle, float x, float y, float z )
{
	SetTargetLocation( handle, Vector3D( x, y, z ) );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetTargetLocation( Handle handle, const Vector3D& location )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		DrawSet& drawSet = m_DrawSets[handle];

		InstanceContainer* pContainer = drawSet.InstanceContainerPointer;
		
		if( pContainer == NULL ) return;
		
		InstanceGlobal* instanceGlobal = pContainer->GetRootInstance();
		instanceGlobal->SetTargetLocation( location );

		drawSet.IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Matrix43 ManagerImplemented::GetBaseMatrix( Handle handle )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		return m_DrawSets[handle].BaseMatrix;
	}

	return Matrix43();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetBaseMatrix( Handle handle, const Matrix43& mat )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].BaseMatrix = mat;
		m_DrawSets[handle].DoUseBaseMatrix = true;
		m_DrawSets[handle].IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetRemovingCallback( Handle handle, EffectInstanceRemovingCallback callback )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].RemovingCallback = callback;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetShown( Handle handle, bool shown )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].IsShown = shown;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetPaused( Handle handle, bool paused )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].IsPaused = paused;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetSpeed( Handle handle, float speed )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].Speed = speed;
		m_DrawSets[handle].IsParameterChanged = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::SetAutoDrawing( Handle handle, bool autoDraw )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].IsAutoDrawing = autoDraw;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Flip()
{
	if( !m_autoFlip )
	{
		m_renderingSession.Enter();
	}

	ExecuteEvents();

	// DrawSet削除処理
	GCDrawSet( false );

	m_renderingDrawSets.clear();
	m_renderingDrawSetMaps.clear();

	/* カリング生成 */
	if( cullingNext.SizeX != cullingCurrent.SizeX ||
		cullingNext.SizeY != cullingCurrent.SizeY ||
		cullingNext.SizeZ != cullingCurrent.SizeZ ||
		cullingNext.LayerCount != cullingCurrent.LayerCount)
	{
		Culling3D::SafeRelease(m_cullingWorld);
		
		std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
		std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();
		while( it != it_end )
		{
			DrawSet& ds = (*it).second;
			Culling3D::SafeRelease(ds.CullingObjectPointer);
			++it;
		}

		m_cullingWorld = Culling3D::World::Create(
			cullingNext.SizeX,
			cullingNext.SizeY,
			cullingNext.SizeZ,
			cullingNext.LayerCount);

		cullingCurrent = cullingNext;
	}

	{
		std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
		std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();
		
		while( it != it_end )
		{
			DrawSet& ds = (*it).second;
			EffectImplemented* effect = (EffectImplemented*)ds.ParameterPointer;

			if(ds.IsParameterChanged)
			{
				if(m_cullingWorld != NULL)
				{
					if(ds.CullingObjectPointer == NULL)
					{
						ds.CullingObjectPointer = Culling3D::Object::Create();
						if(effect->Culling.Shape == CullingShape::Sphere)
						{
							ds.CullingObjectPointer->ChangeIntoSphere(0.0f);
						}

						if (effect->Culling.Shape == CullingShape::NoneShape)
						{
							ds.CullingObjectPointer->ChangeIntoAll();
						}
					}

					InstanceContainer* pContainer = ds.InstanceContainerPointer;
					Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

					Vector3D pos(
						ds.CullingObjectPointer->GetPosition().X,
						ds.CullingObjectPointer->GetPosition().Y,
						ds.CullingObjectPointer->GetPosition().Z);

					Matrix43 pos_;
					pos_.Translation(pos.X, pos.Y, pos.Z);

					Matrix43::Multiple(pos_, pos_,  pInstance->m_GlobalMatrix43);

					if(ds.DoUseBaseMatrix)
					{
						Matrix43::Multiple(pos_, pos_,  ds.BaseMatrix);
					}

					Culling3D::Vector3DF position;
					position.X = pos_.Value[3][0];
					position.Y = pos_.Value[3][1];
					position.Z = pos_.Value[3][2];
					ds.CullingObjectPointer->SetPosition(position);

					if(effect->Culling.Shape == CullingShape::Sphere)
					{
						float radius = effect->Culling.Sphere.Radius;

						{
							Vector3D s;
							pInstance->GetGlobalMatrix43().GetScale(s);
						
							radius = radius * sqrt(s.X * s.X + s.Y * s.Y + s.Z * s.Z);
						}

						if(ds.DoUseBaseMatrix)
						{
							Vector3D s;
							ds.BaseMatrix.GetScale(s);
						
							radius = radius * sqrt(s.X * s.X + s.Y * s.Y + s.Z * s.Z);
						}

						ds.CullingObjectPointer->ChangeIntoSphere(radius);
					}

					m_cullingWorld->AddObject(ds.CullingObjectPointer);
				}
				(*it).second.IsParameterChanged = false;
			}

			m_renderingDrawSets.push_back( (*it).second );
			m_renderingDrawSetMaps[(*it).first] = (*it).second;
			++it;
		}

		if(m_cullingWorld != NULL)
		{
			for(size_t i = 0; i < m_renderingDrawSets.size(); i++)
			{
				m_renderingDrawSets[i].CullingObjectPointer->SetUserData(&(m_renderingDrawSets[i]));
			}
		}
	}

	m_culledObjects.clear();
	m_culledObjectSets.clear();
	m_culled = false;

	if( !m_autoFlip )
	{
		m_renderingSession.Leave();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Update( float deltaFrame )
{
	BeginUpdate();

	// 開始時間を記録
	int64_t beginTime = ::Effekseer::GetTime();

	for( size_t i = 0; i < m_renderingDrawSets.size(); i++ )
	{
		DrawSet& drawSet = m_renderingDrawSets[i];
		
		UpdateHandle( drawSet, deltaFrame );
	}

	// 経過時間を計算
	m_updateTime = (int)(Effekseer::GetTime() - beginTime);

	EndUpdate();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::BeginUpdate()
{
	m_renderingSession.Enter();

	if( m_autoFlip )
	{
		Flip();
	}

	m_sequenceNumber++;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::EndUpdate()
{
	m_renderingSession.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::UpdateHandle( Handle handle, float deltaFrame )
{
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.find( handle );
	if( it != m_DrawSets.end() )
	{
		DrawSet& drawSet = it->second;

		UpdateHandle( drawSet, deltaFrame );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::UpdateHandle( DrawSet& drawSet, float deltaFrame )
{
	if( !drawSet.IsPaused )
	{
		float df = deltaFrame * drawSet.Speed;

		drawSet.InstanceContainerPointer->Update( true, df, drawSet.IsShown );

		if( drawSet.DoUseBaseMatrix )
		{
			drawSet.InstanceContainerPointer->SetBaseMatrix( true, drawSet.BaseMatrix );
		}

		drawSet.GlobalPointer->AddUpdatedFrame( df );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Draw()
{
	m_renderingSession.Enter();

	// 開始時間を記録
	int64_t beginTime = ::Effekseer::GetTime();

	if(m_culled)
	{
		for( size_t i = 0; i < m_culledObjects.size(); i++ )
		{
			DrawSet& drawSet = *m_culledObjects[i];

			if( drawSet.IsShown && drawSet.IsAutoDrawing )
			{
				drawSet.InstanceContainerPointer->Draw( true );
			}
		}
	}
	else
	{
		for( size_t i = 0; i < m_renderingDrawSets.size(); i++ )
		{
			DrawSet& drawSet = m_renderingDrawSets[i];

			if( drawSet.IsShown && drawSet.IsAutoDrawing )
			{
				drawSet.InstanceContainerPointer->Draw( true );
			}
		}
	}

	// 経過時間を計算
	m_drawTime = (int)(Effekseer::GetTime() - beginTime);

	m_renderingSession.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Handle ManagerImplemented::Play( Effect* effect, float x, float y, float z )
{
	if( effect == NULL ) return -1;
	
	// ルート生成
	InstanceGlobal* pGlobal = new InstanceGlobal();
	InstanceContainer* pContainer = CreateInstanceContainer( ((EffectImplemented*)effect)->GetRoot(), pGlobal, true, NULL );
	
	pGlobal->SetRootContainer(  pContainer );

	Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();

	pInstance->m_GlobalMatrix43.Value[3][0] = x;
	pInstance->m_GlobalMatrix43.Value[3][1] = y;
	pInstance->m_GlobalMatrix43.Value[3][2] = z;

	Handle handle = AddDrawSet( effect, pContainer, pGlobal );

	m_DrawSets[handle].GlobalMatrix = pInstance->m_GlobalMatrix43;
	m_DrawSets[handle].IsParameterChanged = true;

	return handle;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::DrawHandle( Handle handle )
{
	m_renderingSession.Enter();
	
	std::map<Handle,DrawSet>::iterator it = m_renderingDrawSetMaps.find( handle );
	if( it != m_renderingDrawSetMaps.end() )
	{
		DrawSet& drawSet = it->second;

		if(m_culled)
		{
			if(m_culledObjectSets.find(drawSet.Self) !=m_culledObjectSets.end())
			{
				if( drawSet.IsShown )
				{
					drawSet.InstanceContainerPointer->Draw( true );
				}
			}
		}
		else
		{
			if( drawSet.IsShown )
			{
				drawSet.InstanceContainerPointer->Draw( true );
			}
		}
	}

	m_renderingSession.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::BeginReloadEffect( Effect* effect )
{
	m_renderingSession.Enter();

	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();

	for(; it != it_end; ++it )
	{
		if( (*it).second.ParameterPointer != effect ) continue;
	
		/* インスタンス削除 */
		(*it).second.InstanceContainerPointer->RemoveForcibly( true );
		(*it).second.InstanceContainerPointer->~InstanceContainer();
		InstanceContainer::operator delete( (*it).second.InstanceContainerPointer, this );
		(*it).second.InstanceContainerPointer = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::EndReloadEffect( Effect* effect )
{
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();

	for(; it != it_end; ++it )
	{
		if( (*it).second.ParameterPointer != effect ) continue;

		/* インスタンス生成 */
		(*it).second.InstanceContainerPointer = CreateInstanceContainer( ((EffectImplemented*)effect)->GetRoot(), (*it).second.GlobalPointer, true, NULL );
		(*it).second.GlobalPointer->SetRootContainer(  (*it).second.InstanceContainerPointer );

		/* 行列設定 */
		(*it).second.InstanceContainerPointer->GetFirstGroup()->GetFirst()->m_GlobalMatrix43 = 
			(*it).second.GlobalMatrix;
		
		/* スキップ */
		for( float f = 0; f < (*it).second.GlobalPointer->GetUpdatedFrame() - 1; f+= 1.0f )
		{
			(*it).second.InstanceContainerPointer->Update( true, 1.0f, false );
		}

		(*it).second.InstanceContainerPointer->Update( true, 1.0f, (*it).second.IsShown );
	}

	m_renderingSession.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::CreateCullingWorld( float xsize, float ysize, float zsize, int32_t layerCount)
{
	cullingNext.SizeX = xsize;
	cullingNext.SizeY = ysize;
	cullingNext.SizeZ = zsize;
	cullingNext.LayerCount = layerCount;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::CalcCulling( const Matrix44& cameraProjMat, bool isOpenGL)
{
	if(m_cullingWorld == NULL) return;

	m_culledObjects.clear();
	m_culledObjectSets.clear();
	
	Matrix44 mat = cameraProjMat;
	mat.Transpose();

	Culling3D::Matrix44* mat_ = (Culling3D::Matrix44*)(&mat);

	m_cullingWorld->Culling(*mat_, isOpenGL);

	for(int32_t i = 0; i < m_cullingWorld->GetObjectCount(); i++)
	{
		Culling3D::Object* o = m_cullingWorld->GetObject(i);
		DrawSet* ds = (DrawSet*)o->GetUserData();

		m_culledObjects.push_back(ds);
		m_culledObjectSets.insert(ds->Self);
	}

	m_culled = true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::RessignCulling()
{
	if (m_cullingWorld == NULL) return;

	m_culledObjects.clear();
	m_culledObjectSets.clear();

	m_cullingWorld->Reassign();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* InstanceContainer::operator new( size_t size, Manager* pManager )
{
	return pManager->GetMallocFunc()( size );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::operator delete( void* p, Manager* pManager )
{
	pManager->GetFreeFunc()( p, sizeof(InstanceContainer) );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer::InstanceContainer( Manager* pManager, EffectNode* pEffectNode, InstanceGlobal* pGlobal, int ChildrenCount )
	: m_pManager		( pManager )
	, m_pGlobal			( pGlobal )
	, m_pEffectNode((EffectNodeImplemented*) pEffectNode)
	, m_Children		( NULL )
	, m_ChildrenCount	( ChildrenCount )
	, m_headGroups		( NULL )
	, m_tailGroups		( NULL )

{
	m_Children = (InstanceContainer**)m_pManager->GetMallocFunc()( sizeof(InstanceContainer*) * m_ChildrenCount );
	for( int i = 0; i < m_ChildrenCount; i++ )
	{
		m_Children[ i ] = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer::~InstanceContainer()
{
	RemoveForcibly( false );

	assert( m_headGroups == NULL );
	assert( m_tailGroups == NULL );

	for( int i = 0; i < m_ChildrenCount; i++ )
	{
		if( m_Children[i] != NULL )
		{
			m_Children[i]->~InstanceContainer();
			InstanceContainer::operator delete( m_Children[i], m_pManager );
			m_Children[i] = NULL;
		}
	}
	m_pManager->GetFreeFunc()( (void*)m_Children, sizeof(InstanceContainer*) * m_ChildrenCount );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer* InstanceContainer::GetChild( int num )
{
	return m_Children[num];
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::SetChild( int num, InstanceContainer* pContainter )
{
	m_Children[num] = pContainter;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::RemoveInvalidGroups()
{
	/* 最後に存在する有効なグループ */
	InstanceGroup* tailGroup = NULL;

	for( InstanceGroup* group = m_headGroups; group != NULL; )
	{
		if( !group->IsReferencedFromInstance && group->GetInstanceCount() == 0  && group->GetRemovingInstanceCount() == 0 )
		{
			InstanceGroup* next = group->NextUsedByContainer;

			delete group;

			if( m_headGroups == group )
			{
				m_headGroups = next;
			}
			group = next;

			if( tailGroup != NULL )
			{
				tailGroup->NextUsedByContainer = next;
			}
		}
		else
		{
			tailGroup = group;
			group = group->NextUsedByContainer;
		}
	}


	m_tailGroups = tailGroup;

	assert( m_tailGroups == NULL || m_tailGroups->NextUsedByContainer == NULL );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGroup* InstanceContainer::CreateGroup()
{
	InstanceGroup* group = new InstanceGroup(m_pManager, m_pEffectNode, this, m_pGlobal);

	if (m_tailGroups != NULL)
	{
		m_tailGroups->NextUsedByContainer = group;
		m_tailGroups = group;
	}
	else
	{
		assert(m_headGroups == NULL);
		m_headGroups = group;
		m_tailGroups = group;
	}

	m_pEffectNode->InitializeRenderedInstanceGroup(*group, m_pManager);

	return group;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGroup* InstanceContainer::GetFirstGroup() const
{
	return m_headGroups;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::Update( bool recursive, float deltaFrame, bool shown )
{
	// 更新
	for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
	{
		group->Update( deltaFrame, shown );
	}
	
	// 破棄
	RemoveInvalidGroups();

	if( recursive )
	{
		for( int i = 0; i < m_ChildrenCount; i++ )
		{
			m_Children[i]->Update( recursive, deltaFrame, shown );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::SetBaseMatrix( bool recursive, const Matrix43& mat )
{
	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT )
	{
		for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
		{
			group->SetBaseMatrix( mat );
		}
	}

	if( recursive )
	{
		for( int i = 0; i < m_ChildrenCount; i++ )
		{
			m_Children[i]->SetBaseMatrix( recursive, mat );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::RemoveForcibly( bool recursive )
{
	KillAllInstances( false );
	
	
	for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
	{
		group->RemoveForcibly();
	}
	RemoveInvalidGroups();

	if( recursive )
	{
		for( int i = 0; i < m_ChildrenCount; i++ )
		{
			m_Children[i]->RemoveForcibly( recursive );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::Draw( bool recursive )
{
	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT && m_pEffectNode->GetType() != EFFECT_NODE_TYPE_NONE )
	{
		/* 個数計測 */
		int32_t count = 0;
		{
			for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
			{
				for (auto& instance : group->m_instances)
				{
					if (instance->m_State == INSTANCE_STATE_ACTIVE)
					{
						count++;
					}
				}
			}
		}

		if( count > 0 )
		{
			/* 描画 */
			m_pEffectNode->BeginRendering(count, m_pManager);

			for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
			{
				m_pEffectNode->BeginRenderingGroup(group, m_pManager);

				if( m_pEffectNode->RenderingOrder == RenderingOrder_FirstCreatedInstanceIsFirst )
				{
					for (auto& instance : group->m_instances)
					{
						if (instance->m_State == INSTANCE_STATE_ACTIVE)
						{
							instance->Draw();
						}
					}
				}
				else
				{
					std::list<Instance*>::reverse_iterator it = group->m_instances.rbegin();

					while( it != group->m_instances.rend() )
					{
						if( (*it)->m_State == INSTANCE_STATE_ACTIVE )
						{
							(*it)->Draw();
						}
						it++;
					}
				}
			}

			m_pEffectNode->EndRendering(m_pManager);
		}
	}

	if( recursive )
	{
		for( int i = 0; i < m_ChildrenCount; i++ )
		{
			m_Children[i]->Draw( recursive );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::KillAllInstances( bool recursive )
{
	for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer )
	{
		group->KillAllInstances();
	}

	if( recursive )
	{
		for( int i = 0; i < m_ChildrenCount; i++ )
		{
			m_Children[i]->KillAllInstances( recursive );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGlobal* InstanceContainer::GetRootInstance()
{
	return m_pGlobal;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Instance::Instance( Manager* pManager, EffectNode* pEffectNode, InstanceContainer* pContainer )
	: m_pManager			( pManager )
	, m_pEffectNode((EffectNodeImplemented*) pEffectNode)
	, m_pContainer			( pContainer )
	, m_headGroups		( NULL )
	, m_pParent			( NULL )
	, m_State			( INSTANCE_STATE_ACTIVE )
	, m_LivedTime		( 0 )
	, m_LivingTime		( 0 )
	, m_stepTime		( false )
	, m_sequenceNumber	( 0 )

{
	InstanceGroup* group = NULL;

	for( int i = 0; i < m_pEffectNode->GetChildrenCount(); i++ )
	{
		InstanceContainer* pContainer = m_pContainer->GetChild( i );

		if( group != NULL )
		{
			group->NextUsedByInstance = pContainer->CreateGroup();
			group = group->NextUsedByInstance;
		}
		else
		{
			group = pContainer->CreateGroup();
			m_headGroups = group;
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Instance::~Instance()
{
	assert( m_State != INSTANCE_STATE_ACTIVE );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
eInstanceState Instance::GetState() const
{
	return m_State;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const Matrix43& Instance::GetGlobalMatrix43() const
{
	return m_GlobalMatrix43;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Initialize( Instance* parent, int32_t instanceNumber )
{
	auto parameter = (EffectNodeImplemented*) m_pEffectNode;

	// 親の設定
	m_pParent = parent;

	// 子の初期化
	for (int32_t i = 0; i < Min(ChildrenMax, parameter->GetChildrenCount()); i++)
	{
		auto pNode = (EffectNodeImplemented*) parameter->GetChild(i);

		m_generatedChildrenCount[i] = 0;
		m_nextGenerationTime[i] = pNode->CommonValues.GenerationTimeOffset.getValue(*m_pManager);
	}

	if( m_pParent == NULL )
	{
		// ROOTの場合

		// 状態の初期化
		m_State = INSTANCE_STATE_ACTIVE;

		// 時間周りの初期化
		m_LivingTime = 0.0f;
		m_LivedTime = FLT_MAX;

		// SRTの初期化
		m_GenerationLocation.Indentity();
		m_GlobalMatrix43.Indentity();
		m_ParentMatrix43.Indentity();

		// 親の初期化
		m_ParentMatrix43 = GetGlobalMatrix43();

		return;
	}

	// 状態の初期化
	m_State = INSTANCE_STATE_ACTIVE;

	// 時間周りの初期化
	m_LivingTime = 0.0f;
	m_LivedTime = (float)parameter->CommonValues.life.getValue( *m_pManager );


	// SRTの初期化
	m_pParent->GetGlobalMatrix43().GetTranslation( m_GlobalPosition );
	m_GlobalRevisionLocation = Vector3D(0.0f, 0.0f, 0.0f);
	m_GlobalRevisionVelocity = Vector3D(0.0f, 0.0f, 0.0f);
	m_GenerationLocation.Indentity();
	m_GlobalMatrix43.Indentity();
	m_ParentMatrix43.Indentity();

	// 親の初期化
	if( parameter->CommonValues.TranslationBindType == BindType_WhenCreating )
	{
		m_ParentMatrix43.Value[3][0] = m_pParent->m_GlobalMatrix43.Value[3][0];
		m_ParentMatrix43.Value[3][1] = m_pParent->m_GlobalMatrix43.Value[3][1];
		m_ParentMatrix43.Value[3][2] = m_pParent->m_GlobalMatrix43.Value[3][2];
	}

	if( parameter->CommonValues.RotationBindType == BindType_WhenCreating &&
		parameter->CommonValues.ScalingBindType == BindType_WhenCreating )
	{
		for( int m = 0; m < 3; m++ )
		{
			for( int n = 0; n < 3; n++ )
			{
				m_ParentMatrix43.Value[m][n] = m_pParent->m_GlobalMatrix43.Value[m][n];
			}
		}
	}
	else if ( parameter->CommonValues.RotationBindType == BindType_WhenCreating )
	{
		for( int m = 0; m < 3; m++ )
		{
			for( int n = 0; n < 3; n++ )
			{
				m_ParentMatrix43.Value[m][n] = m_pParent->m_GlobalMatrix43.Value[m][n];
			}
		}

		float s[3];
		for( int m = 0; m < 3; m++ )
		{
			s[m] = 0;
			for( int n = 0; n < 3; n++ )
			{
				s[m] += m_ParentMatrix43.Value[m][n] * m_ParentMatrix43.Value[m][n];
			}
			s[m] = sqrt( s[m] );
		}

		for( int m = 0; m < 3; m++ )
		{
			for( int n = 0; n < 3; n++ )
			{
				m_ParentMatrix43.Value[m][n] = m_ParentMatrix43.Value[m][n] / s[m];
			}
		}
	}
	else if ( parameter->CommonValues.ScalingBindType == BindType_WhenCreating )
	{
		float s[3];
		for( int m = 0; m < 3; m++ )
		{
			s[m] = 0;
			for( int n = 0; n < 3; n++ )
			{
				s[m] += m_pParent->m_GlobalMatrix43.Value[m][n] * m_pParent->m_GlobalMatrix43.Value[m][n];
			}
			s[m] = sqrt( s[m] );
		}
		m_ParentMatrix43.Value[0][0] = s[0];
		m_ParentMatrix43.Value[1][1] = s[1];
		m_ParentMatrix43.Value[2][2] = s[2];
	}
	
	/* 位置 */
	if( m_pEffectNode->TranslationType == ParameterTranslationType_Fixed )
	{
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_PVA )
	{
		translation_values.random.location = m_pEffectNode->TranslationPVA.location.getValue( *m_pManager );
		translation_values.random.velocity = m_pEffectNode->TranslationPVA.velocity.getValue( *m_pManager );
		translation_values.random.acceleration = m_pEffectNode->TranslationPVA.acceleration.getValue( *m_pManager );
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_Easing )
	{
		translation_values.easing.start = m_pEffectNode->TranslationEasing.start.getValue( *m_pManager );
		translation_values.easing.end = m_pEffectNode->TranslationEasing.end.getValue( *m_pManager );
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_FCurve )
	{
		assert( m_pEffectNode->TranslationFCurve != NULL );

		translation_values.fcruve.offset.x = m_pEffectNode->TranslationFCurve->X.GetOffset( *m_pManager );
		translation_values.fcruve.offset.y = m_pEffectNode->TranslationFCurve->Y.GetOffset( *m_pManager );
		translation_values.fcruve.offset.z = m_pEffectNode->TranslationFCurve->Z.GetOffset( *m_pManager );
	}
	
	/* 回転 */
	if( m_pEffectNode->RotationType == ParameterRotationType_Fixed )
	{
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_PVA )
	{
		rotation_values.random.rotation = m_pEffectNode->RotationPVA.rotation.getValue( *m_pManager );
		rotation_values.random.velocity = m_pEffectNode->RotationPVA.velocity.getValue( *m_pManager );
		rotation_values.random.acceleration = m_pEffectNode->RotationPVA.acceleration.getValue( *m_pManager );
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_Easing )
	{
		rotation_values.easing.start = m_pEffectNode->RotationEasing.start.getValue( *m_pManager );
		rotation_values.easing.end = m_pEffectNode->RotationEasing.end.getValue( *m_pManager );
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_AxisPVA )
	{
		rotation_values.axis.random.rotation = m_pEffectNode->RotationAxisPVA.rotation.getValue( *m_pManager );
		rotation_values.axis.random.velocity = m_pEffectNode->RotationAxisPVA.velocity.getValue( *m_pManager );
		rotation_values.axis.random.acceleration = m_pEffectNode->RotationAxisPVA.acceleration.getValue( *m_pManager );
		rotation_values.axis.rotation = rotation_values.axis.random.rotation;
		rotation_values.axis.axis = m_pEffectNode->RotationAxisPVA.axis.getValue(*m_pManager);
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_AxisEasing )
	{
		rotation_values.axis.easing.start = m_pEffectNode->RotationAxisEasing.easing.start.getValue( *m_pManager );
		rotation_values.axis.easing.end = m_pEffectNode->RotationAxisEasing.easing.end.getValue( *m_pManager );
		rotation_values.axis.rotation = rotation_values.axis.easing.start;
		rotation_values.axis.axis = m_pEffectNode->RotationAxisEasing.axis.getValue(*m_pManager);
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_FCurve )
	{
		assert( m_pEffectNode->RotationFCurve != NULL );

		rotation_values.fcruve.offset.x = m_pEffectNode->RotationFCurve->X.GetOffset( *m_pManager );
		rotation_values.fcruve.offset.y = m_pEffectNode->RotationFCurve->Y.GetOffset( *m_pManager );
		rotation_values.fcruve.offset.z = m_pEffectNode->RotationFCurve->Z.GetOffset( *m_pManager );
	}

	/* 拡大縮小 */
	if( m_pEffectNode->ScalingType == ParameterScalingType_Fixed )
	{
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_PVA )
	{
		scaling_values.random.scale = m_pEffectNode->ScalingPVA.Position.getValue( *m_pManager );
		scaling_values.random.velocity = m_pEffectNode->ScalingPVA.Velocity.getValue( *m_pManager );
		scaling_values.random.acceleration = m_pEffectNode->ScalingPVA.Acceleration.getValue( *m_pManager );
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_Easing )
	{
		scaling_values.easing.start = m_pEffectNode->ScalingEasing.start.getValue( *m_pManager );
		scaling_values.easing.end = m_pEffectNode->ScalingEasing.end.getValue( *m_pManager );
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_SinglePVA )
	{
		scaling_values.single_random.scale = m_pEffectNode->ScalingSinglePVA.Position.getValue( *m_pManager );
		scaling_values.single_random.velocity = m_pEffectNode->ScalingSinglePVA.Velocity.getValue( *m_pManager );
		scaling_values.single_random.acceleration = m_pEffectNode->ScalingSinglePVA.Acceleration.getValue( *m_pManager );
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_SingleEasing )
	{
		scaling_values.single_easing.start = m_pEffectNode->ScalingSingleEasing.start.getValue( *m_pManager );
		scaling_values.single_easing.end = m_pEffectNode->ScalingSingleEasing.end.getValue( *m_pManager );
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_FCurve )
	{
		assert( m_pEffectNode->ScalingFCurve != NULL );

		scaling_values.fcruve.offset.x = m_pEffectNode->ScalingFCurve->X.GetOffset( *m_pManager );
		scaling_values.fcruve.offset.y = m_pEffectNode->ScalingFCurve->Y.GetOffset( *m_pManager );
		scaling_values.fcruve.offset.z = m_pEffectNode->ScalingFCurve->Z.GetOffset( *m_pManager );
	}

	/* 生成位置 */
	if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT )
	{
		vector3d p = m_pEffectNode->GenerationLocation.point.location.getValue( *m_pManager );
		m_GenerationLocation.Translation( p.x, p.y, p.z );
	}
	else if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE )
	{
		Matrix43 mat_x, mat_y;
		mat_x.RotationX( m_pEffectNode->GenerationLocation.sphere.rotation_x.getValue( *m_pManager ) );
		mat_y.RotationY( m_pEffectNode->GenerationLocation.sphere.rotation_y.getValue( *m_pManager ) );
		float r = m_pEffectNode->GenerationLocation.sphere.radius.getValue( *m_pManager );
		m_GenerationLocation.Translation( 0, r, 0 );
		Matrix43::Multiple( m_GenerationLocation, m_GenerationLocation, mat_x );
		Matrix43::Multiple( m_GenerationLocation, m_GenerationLocation, mat_y );
	}
	else if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_MODEL )
	{
		m_GenerationLocation.Indentity();

		int32_t modelIndex = m_pEffectNode->GenerationLocation.model.index;
		if( modelIndex >= 0 )
		{
			Model* model = (Model*)m_pEffectNode->GetEffect()->GetModel( modelIndex );
			if( model != NULL )
			{
				Model::Emitter emitter;
				
				if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_RANDOM )
				{
					emitter = model->GetEmitter( 
						m_pManager, 
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_VERTEX )
				{
					emitter = model->GetEmitterFromVertex( 
						instanceNumber,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_VERTEX_RANDOM )
				{
					emitter = model->GetEmitterFromVertex( 
						m_pManager,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_FACE )
				{
					emitter = model->GetEmitterFromFace( 
						instanceNumber,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_FACE_RANDOM )
				{
					emitter = model->GetEmitterFromFace( 
						m_pManager,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}

				m_GenerationLocation.Translation( 
					emitter.Position.X, 
					emitter.Position.Y,
					emitter.Position.Z );

				if( m_pEffectNode->GenerationLocation.EffectsRotation )
				{
					m_GenerationLocation.Value[0][0] = emitter.Binormal.X;
					m_GenerationLocation.Value[0][1] = emitter.Binormal.Y;
					m_GenerationLocation.Value[0][2] = emitter.Binormal.Z;

					m_GenerationLocation.Value[1][0] = emitter.Tangent.X;
					m_GenerationLocation.Value[1][1] = emitter.Tangent.Y;
					m_GenerationLocation.Value[1][2] = emitter.Tangent.Z;

					m_GenerationLocation.Value[2][0] = emitter.Normal.X;
					m_GenerationLocation.Value[2][1] = emitter.Normal.Y;
					m_GenerationLocation.Value[2][2] = emitter.Normal.Z;
				}
			}
		}
	}
	else if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_CIRCLE )
	{
		m_GenerationLocation.Indentity();
		float radius = m_pEffectNode->GenerationLocation.circle.radius.getValue(*m_pManager);
		float start = m_pEffectNode->GenerationLocation.circle.angle_start.getValue(*m_pManager);
		float end = m_pEffectNode->GenerationLocation.circle.angle_end.getValue(*m_pManager);
		int32_t div = Max(m_pEffectNode->GenerationLocation.circle.division, 1);

		int32_t target = 0;
		if(m_pEffectNode->GenerationLocation.circle.type == ParameterGenerationLocation::CIRCLE_TYPE_ORDER)
		{
			target = instanceNumber % div;
		}
		else if(m_pEffectNode->GenerationLocation.circle.type == ParameterGenerationLocation::CIRCLE_TYPE_REVERSE_ORDER)
		{
			target = div - 1 - (instanceNumber % div);
		}
		else if(m_pEffectNode->GenerationLocation.circle.type == ParameterGenerationLocation::CIRCLE_TYPE_RANDOM)
		{
			RandFunc randFunc = m_pManager->GetRandFunc();
			int32_t randMax = m_pManager->GetRandMax();

			target = (int32_t)( (div) * ( (float)randFunc() / (float)randMax ) );
			if (target == div) div -= 1;
		}

		float angle = (end - start) * ((float)target / (float)div) + start;
		Matrix43 mat;
		mat.RotationZ( angle );

		m_GenerationLocation.Translation( 0, radius, 0 );
		Matrix43::Multiple( m_GenerationLocation, m_GenerationLocation, mat );
	}

	if( m_pEffectNode->SoundType == ParameterSoundType_Use )
	{
		soundValues.delay = m_pEffectNode->Sound.Delay.getValue( *m_pManager );
	}

	m_pEffectNode->InitializeRenderedInstance( *this, m_pManager );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Update( float deltaFrame, bool shown )
{
	if (m_stepTime && m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT)
	{
		/* 音の更新(現状放置) */
		if (m_pEffectNode->SoundType == ParameterSoundType_Use)
		{
			float living_time = m_LivingTime;
			float living_time_p = living_time + deltaFrame;

			if (living_time <= (float) soundValues.delay && (float) soundValues.delay < living_time_p)
			{
				m_pEffectNode->PlaySound_(*this, m_pContainer->GetRootInstance(), m_pManager);
			}
		}
	}

	float originalTime = m_LivingTime;
	bool calculateMatrix = false;

	if(shown)
	{
		calculateMatrix = true;
	}
	else if( m_pEffectNode->LocationAbs.type != LocationAbsParameter::None )
	{
		// 絶対位置が設定されている場合は毎回計算が必要
		calculateMatrix = true;
	}
	else
	{
		/**
			見えないケースで行列計算が必要なケース
			-子が生成される。
			-子の子が生成される。
			*/
		if (m_stepTime && (originalTime <= m_LivedTime || !m_pEffectNode->CommonValues.RemoveWhenLifeIsExtinct))
		{
			for (int i = 0; i < Min(ChildrenMax, m_pEffectNode->GetChildrenCount()); i++)
			{
				auto pNode = (EffectNodeImplemented*) m_pEffectNode->GetChild(i);

				// インスタンス生成
				if (pNode->CommonValues.MaxGeneration > m_generatedChildrenCount[i] &&
					originalTime + deltaFrame > m_nextGenerationTime[i])
				{
					calculateMatrix = true;
					break;
				}
			}
		}
	}

	/* 親が破棄される瞬間に行列計算(条件を絞れば更に最適化可能) */
	if( !calculateMatrix && m_pParent != NULL && m_pParent->GetState() != INSTANCE_STATE_ACTIVE &&
		!(m_pEffectNode->CommonValues.RemoveWhenParentIsRemoved && m_pEffectNode->GetChildrenCount() == 0))
	{
		calculateMatrix = true;
	}

	if( calculateMatrix )
	{
		CalculateMatrix( deltaFrame );
	}

	/* 親の削除処理 */
	if (m_pParent != NULL && m_pParent->GetState() != INSTANCE_STATE_ACTIVE)
	{
		m_pParent = nullptr;
	}

	/* 時間の進行 */
	if(  m_stepTime )
	{
		m_LivingTime += deltaFrame;
	}

	// 子の処理
	if( m_stepTime && (originalTime <= m_LivedTime || !m_pEffectNode->CommonValues.RemoveWhenLifeIsExtinct) )
	{
		InstanceGroup* group = m_headGroups;

		for (int i = 0; i < Min(ChildrenMax, m_pEffectNode->GetChildrenCount()); i++, group = group->NextUsedByInstance)
		{
			auto pNode = (EffectNodeImplemented*) m_pEffectNode->GetChild(i);
			auto pContainer = m_pContainer->GetChild( i );
			assert( group != NULL );

			// インスタンス生成
			while (true)
			{
				if (pNode->CommonValues.MaxGeneration > m_generatedChildrenCount[i] &&
					originalTime + deltaFrame > m_nextGenerationTime[i])
				{
					// 生成処理
					Instance* pNewInstance = group->CreateInstance();
					if (pNewInstance != NULL)
					{
						pNewInstance->Initialize(this, m_generatedChildrenCount[i]);
					}
					else
					{
						break;
					}

					m_generatedChildrenCount[i]++;
					m_nextGenerationTime[i] += Max(0.0f, pNode->CommonValues.GenerationTime.getValue(*m_pManager));
				}
				else
				{
					break;
				}
			}
		}
	}
	
	// 死亡判定
	bool killed = false;
	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT )
	{
		// 時間経過
		if( m_pEffectNode->CommonValues.RemoveWhenLifeIsExtinct )
		{
			if( m_LivingTime > m_LivedTime )
			{
				killed = true;
			}
		}

		// 親が消えた場合
		if( m_pEffectNode->CommonValues.RemoveWhenParentIsRemoved )
		{
			if( m_pParent == nullptr || m_pParent->GetState() != INSTANCE_STATE_ACTIVE )
			{
				m_pParent = nullptr;
				killed = true;
			}
		}

		// 子が全て消えた場合
		if( !killed && m_pEffectNode->CommonValues.RemoveWhenChildrenIsExtinct )
		{
			int maxcreate_count = 0;
			InstanceGroup* group = m_headGroups;

			for (int i = 0; i < Min(ChildrenMax, m_pEffectNode->GetChildrenCount()); i++, group = group->NextUsedByInstance)
			{
				auto child = (EffectNodeImplemented*) m_pEffectNode->GetChild(i);

				float last_generation_time = 
					child->CommonValues.GenerationTime.max *
					(child->CommonValues.MaxGeneration - 1) +
					child->CommonValues.GenerationTimeOffset.max +
					1.0f;

				if( m_LivingTime >= last_generation_time && group->GetInstanceCount() == 0 )
				{
					maxcreate_count++;
				}
				else
				{
					break;
				}
			}
			
			if( maxcreate_count == m_pEffectNode->GetChildrenCount() )
			{
				killed = true;
			}
		}
	}

	if(killed)
	{
		/* 死亡確定時、計算が必要な場合は計算をする。*/
		if( !calculateMatrix &&
			m_pEffectNode->GetChildrenCount() > 0)
		{
			calculateMatrix = true;
			CalculateMatrix( deltaFrame );
		}

		/* 破棄 */
		Kill();
		return;
	}

	// 時間の進行許可
	m_stepTime = true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::CalculateMatrix( float deltaFrame )
{
	if( m_sequenceNumber == ((ManagerImplemented*)m_pManager)->GetSequenceNumber() ) return;
	m_sequenceNumber = ((ManagerImplemented*)m_pManager)->GetSequenceNumber();

	assert( m_pEffectNode != NULL );
	assert( m_pContainer != NULL );

	// 親の処理
	if( m_pParent != NULL )
	{
		CalculateParentMatrix();
	}

	Vector3D localPosition;
	Vector3D localAngle;
	Vector3D localScaling;

	/* 更新処理 */
	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT )
	{
		/* 位置の更新(時間から直接求めれるよう対応済み) */
		if( m_pEffectNode->TranslationType == ParameterTranslationType_None )
		{
			localPosition.X = 0;
			localPosition.Y = 0;
			localPosition.Z = 0;
		}
		else if( m_pEffectNode->TranslationType == ParameterTranslationType_Fixed )
		{
			localPosition.X = m_pEffectNode->TranslationFixed.Position.X;
			localPosition.Y = m_pEffectNode->TranslationFixed.Position.Y;
			localPosition.Z = m_pEffectNode->TranslationFixed.Position.Z;
		}
		else if( m_pEffectNode->TranslationType == ParameterTranslationType_PVA )
		{
			/* 現在位置 = 初期座標 + (初期速度 * t) + (初期加速度 * t * t * 0.5)*/
			localPosition.X = translation_values.random.location.x +
				(translation_values.random.velocity.x * m_LivingTime) +
				(translation_values.random.acceleration.x * m_LivingTime * m_LivingTime * 0.5f);

			localPosition.Y = translation_values.random.location.y +
				(translation_values.random.velocity.y * m_LivingTime) +
				(translation_values.random.acceleration.y * m_LivingTime * m_LivingTime * 0.5f);

			localPosition.Z = translation_values.random.location.z +
				(translation_values.random.velocity.z * m_LivingTime) +
				(translation_values.random.acceleration.z * m_LivingTime * m_LivingTime * 0.5f);

		}
		else if( m_pEffectNode->TranslationType == ParameterTranslationType_Easing )
		{
			m_pEffectNode->TranslationEasing.setValueToArg(
				localPosition,
				translation_values.easing.start,
				translation_values.easing.end,
				m_LivingTime / m_LivedTime );
		}
		else if( m_pEffectNode->TranslationType == ParameterTranslationType_FCurve )
		{
			assert( m_pEffectNode->TranslationFCurve != NULL );
			localPosition.X = m_pEffectNode->TranslationFCurve->X.GetValue( (int)m_LivingTime ) + translation_values.fcruve.offset.x;
			localPosition.Y = m_pEffectNode->TranslationFCurve->Y.GetValue( (int)m_LivingTime ) + translation_values.fcruve.offset.y;
			localPosition.Z = m_pEffectNode->TranslationFCurve->Z.GetValue( (int)m_LivingTime ) + translation_values.fcruve.offset.z;
		}

		if( !m_pEffectNode->GenerationLocation.EffectsRotation )
		{
			localPosition.X += m_GenerationLocation.Value[3][0];
			localPosition.Y += m_GenerationLocation.Value[3][1];
			localPosition.Z += m_GenerationLocation.Value[3][2];
		}

		/* 回転の更新(時間から直接求めれるよう対応済み) */
		if( m_pEffectNode->RotationType == ParameterRotationType_None )
		{
			localAngle.X = 0;
			localAngle.Y = 0;
			localAngle.Z = 0;
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_Fixed )
		{
			localAngle.X = m_pEffectNode->RotationFixed.Position.X;
			localAngle.Y = m_pEffectNode->RotationFixed.Position.Y;
			localAngle.Z = m_pEffectNode->RotationFixed.Position.Z;
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_PVA )
		{
			/* 現在位置 = 初期座標 + (初期速度 * t) + (初期加速度 * t * t * 0.5)*/
			localAngle.X = rotation_values.random.rotation.x +
				(rotation_values.random.velocity.x * m_LivingTime) +
				(rotation_values.random.acceleration.x * m_LivingTime * m_LivingTime * 0.5f);

			localAngle.Y = rotation_values.random.rotation.y +
				(rotation_values.random.velocity.y * m_LivingTime) +
				(rotation_values.random.acceleration.y * m_LivingTime * m_LivingTime * 0.5f);

			localAngle.Z = rotation_values.random.rotation.z +
				(rotation_values.random.velocity.z * m_LivingTime) +
				(rotation_values.random.acceleration.z * m_LivingTime * m_LivingTime * 0.5f);

		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_Easing )
		{
			m_pEffectNode->RotationEasing.setValueToArg(
				localAngle,
				rotation_values.easing.start,
				rotation_values.easing.end,
				m_LivingTime / m_LivedTime );
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_AxisPVA )
		{
			rotation_values.axis.rotation = 
				rotation_values.axis.random.rotation +
				rotation_values.axis.random.velocity * m_LivingTime +
				rotation_values.axis.random.acceleration * m_LivingTime * m_LivingTime * 0.5f;
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_AxisEasing )
		{
			m_pEffectNode->RotationAxisEasing.easing.setValueToArg(
				rotation_values.axis.rotation,
				rotation_values.axis.easing.start,
				rotation_values.axis.easing.end,
				m_LivingTime / m_LivedTime );
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_FCurve )
		{
			assert( m_pEffectNode->RotationFCurve != NULL );
			localAngle.X = m_pEffectNode->RotationFCurve->X.GetValue( (int)m_LivingTime ) + rotation_values.fcruve.offset.x;
			localAngle.Y = m_pEffectNode->RotationFCurve->Y.GetValue( (int)m_LivingTime ) + rotation_values.fcruve.offset.y;
			localAngle.Z = m_pEffectNode->RotationFCurve->Z.GetValue( (int)m_LivingTime ) + rotation_values.fcruve.offset.z;
		}

		/* 拡大の更新(時間から直接求めれるよう対応済み) */
		if( m_pEffectNode->ScalingType == ParameterScalingType_None )
		{
			localScaling.X = 1.0f;
			localScaling.Y = 1.0f;
			localScaling.Z = 1.0f;
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_Fixed )
		{
			localScaling.X = m_pEffectNode->ScalingFixed.Position.X;
			localScaling.Y = m_pEffectNode->ScalingFixed.Position.Y;
			localScaling.Z = m_pEffectNode->ScalingFixed.Position.Z;
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_PVA )
		{
			/* 現在位置 = 初期座標 + (初期速度 * t) + (初期加速度 * t * t * 0.5)*/
			localScaling.X = scaling_values.random.scale.x +
				(scaling_values.random.velocity.x * m_LivingTime) +
				(scaling_values.random.acceleration.x * m_LivingTime * m_LivingTime * 0.5f);

			localScaling.Y = scaling_values.random.scale.y +
				(scaling_values.random.velocity.y * m_LivingTime) +
				(scaling_values.random.acceleration.y * m_LivingTime * m_LivingTime * 0.5f);

			localScaling.Z = scaling_values.random.scale.z +
				(scaling_values.random.velocity.z * m_LivingTime) +
				(scaling_values.random.acceleration.z * m_LivingTime * m_LivingTime * 0.5f);
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_Easing )
		{
			m_pEffectNode->ScalingEasing.setValueToArg(
				localScaling,
				scaling_values.easing.start,
				scaling_values.easing.end,
				m_LivingTime / m_LivedTime );
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_SinglePVA )
		{
			float s = scaling_values.single_random.scale +
				scaling_values.single_random.velocity * m_LivingTime +
				scaling_values.single_random.acceleration * m_LivingTime * m_LivingTime * 0.5f;
			localScaling.X = s;
			localScaling.Y = s;
			localScaling.Z = s;
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_SingleEasing )
		{
			float scale;
			m_pEffectNode->ScalingSingleEasing.setValueToArg(
				scale,
				scaling_values.single_easing.start,
				scaling_values.single_easing.end,
				m_LivingTime / m_LivedTime );
			localScaling.X = scale;
			localScaling.Y = scale;
			localScaling.Z = scale;
		}
		else if( m_pEffectNode->ScalingType == ParameterScalingType_FCurve )
		{
			assert( m_pEffectNode->ScalingFCurve != NULL );

			localScaling.X = m_pEffectNode->ScalingFCurve->X.GetValue( (int32_t)m_LivingTime ) + scaling_values.fcruve.offset.x;
			localScaling.Y = m_pEffectNode->ScalingFCurve->Y.GetValue( (int32_t)m_LivingTime ) + scaling_values.fcruve.offset.y;
			localScaling.Z = m_pEffectNode->ScalingFCurve->Z.GetValue( (int32_t)m_LivingTime ) + scaling_values.fcruve.offset.z;
		}

		/* 描画部分の更新 */
		m_pEffectNode->UpdateRenderedInstance( *this, m_pManager );
	}
	
	// 行列の更新
	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT )
	{
		m_GlobalMatrix43.Scaling( localScaling.X, localScaling.Y,  localScaling.Z );

		if( m_pEffectNode->RotationType == ParameterRotationType_Fixed ||
			m_pEffectNode->RotationType == ParameterRotationType_PVA ||
			m_pEffectNode->RotationType == ParameterRotationType_Easing ||
			m_pEffectNode->RotationType == ParameterRotationType_FCurve )
		{
			Matrix43 MatRot;
			MatRot.RotationZXY( localAngle.Z, localAngle.X, localAngle.Y );
			Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, MatRot );
		}
		else if( m_pEffectNode->RotationType == ParameterRotationType_AxisPVA ||
				 m_pEffectNode->RotationType == ParameterRotationType_AxisEasing )
		{
			Matrix43 MatRot;
			Vector3D axis;
			axis.X = rotation_values.axis.axis.x;
			axis.Y = rotation_values.axis.axis.y;
			axis.Z = rotation_values.axis.axis.z;

			MatRot.RotationAxis( axis, rotation_values.axis.rotation );
			Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, MatRot );
		}

		if( localPosition.X != 0.0f ||
			localPosition.Y != 0.0f || 
			localPosition.Z != 0.0f )
		{
			Matrix43 MatTra;
			MatTra.Translation( localPosition.X, localPosition.Y, localPosition.Z );
			Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, MatTra );
		}

		if( m_pEffectNode->GenerationLocation.EffectsRotation )
		{
			Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, m_GenerationLocation );
		}

		Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, m_ParentMatrix43 );
		
		Vector3D currentPosition;
		m_GlobalMatrix43.GetTranslation( currentPosition );
		m_GlobalVelocity = currentPosition - m_GlobalPosition;
		m_GlobalPosition = currentPosition;

		if( m_pEffectNode->LocationAbs.type != LocationAbsParameter::None )
		{
			ModifyMatrixFromLocationAbs( deltaFrame );
		}
	}
}

void Instance::CalculateParentMatrix()
{
	/* 親の行列を更新(現在は必要不必要関わらず行なっている) */
	//m_pParent->CalculateMatrix( deltaFrame );

	if( m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT )
	{
		BindType lType = m_pEffectNode->CommonValues.TranslationBindType;
		BindType rType = m_pEffectNode->CommonValues.RotationBindType;
		BindType sType = m_pEffectNode->CommonValues.ScalingBindType;

		const Instance* rootInstance;
		InstanceGlobal* instanceGlobal = m_pContainer->GetRootInstance();
		InstanceContainer* rootContainer = instanceGlobal->GetRootContainer();
		InstanceGroup* firstGloup = rootContainer->GetFirstGroup();
		if( firstGloup && firstGloup->GetInstanceCount() > 0 )
		{
			rootInstance = firstGloup->GetFirst();
		}
		else
		{
			rootInstance = NULL;
		}
		
		if( (lType == BindType_Always && rType == BindType_Always && sType == BindType_Always) )
		{
			m_ParentMatrix43 = m_pParent->GetGlobalMatrix43();
		}
		else if ( lType == BindType_NotBind_Root && rType == BindType_NotBind_Root && sType == BindType_NotBind_Root )
		{
			m_ParentMatrix43 = rootInstance->GetGlobalMatrix43();
		}
		else
		{
			Vector3D s, t;
			Matrix43 r;

			m_ParentMatrix43.GetSRT( s, r, t );
			
			if( lType == BindType_Always )
			{
				m_pParent->GetGlobalMatrix43().GetTranslation( t );
			}
			else if( lType == BindType_NotBind_Root && rootInstance != NULL )
			{
				rootInstance->GetGlobalMatrix43().GetTranslation( t );
			}
			
			if( rType == BindType_Always )
			{
				m_pParent->GetGlobalMatrix43().GetRotation( r );
			}
			else if( rType == BindType_NotBind_Root && rootInstance != NULL )
			{
				rootInstance->GetGlobalMatrix43().GetRotation( r );
			}
			

			if( sType == BindType_Always )
			{
				m_pParent->GetGlobalMatrix43().GetScale( s );
			}
			else if( sType == BindType_NotBind_Root && rootInstance != NULL )
			{
				rootInstance->GetGlobalMatrix43().GetScale( s );
			}

			m_ParentMatrix43.SetSRT( s, r, t );
		}
	}
	else
	{
		// Rootの場合
		m_ParentMatrix43 = m_pParent->GetGlobalMatrix43();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::ModifyMatrixFromLocationAbs( float deltaFrame )
{
	InstanceGlobal* instanceGlobal = m_pContainer->GetRootInstance();

	/* 絶対位置の更新(時間から直接求めれるよう対応済み) */
	if( m_pEffectNode->LocationAbs.type == LocationAbsParameter::None )
	{	
	}
	else if( m_pEffectNode->LocationAbs.type == LocationAbsParameter::Gravity )
	{
		m_GlobalRevisionLocation.X = m_pEffectNode->LocationAbs.gravity.x *
			m_LivingTime * m_LivingTime * 0.5f;
		m_GlobalRevisionLocation.Y = m_pEffectNode->LocationAbs.gravity.y *
			m_LivingTime * m_LivingTime * 0.5f;
		m_GlobalRevisionLocation.Z = m_pEffectNode->LocationAbs.gravity.z *
			m_LivingTime * m_LivingTime * 0.5f;
	}
	else if( m_pEffectNode->LocationAbs.type == LocationAbsParameter::AttractiveForce )
	{
		InstanceGlobal* instanceGlobal = m_pContainer->GetRootInstance();

		float force = m_pEffectNode->LocationAbs.attractiveForce.force;
		float control = m_pEffectNode->LocationAbs.attractiveForce.control;
		float minRange = m_pEffectNode->LocationAbs.attractiveForce.minRange;
		float maxRange = m_pEffectNode->LocationAbs.attractiveForce.maxRange;
		
		Vector3D position = m_GlobalPosition - m_GlobalVelocity + m_GlobalRevisionLocation;

		Vector3D targetDifference = instanceGlobal->GetTargetLocation() - position;
		float targetDistance = Vector3D::Length( targetDifference );
		if( targetDistance > 0.0f )
		{
			Vector3D targetDirection = targetDifference / targetDistance;
		
			if( minRange > 0.0f || maxRange > 0.0f )
			{
				if( targetDistance >= m_pEffectNode->LocationAbs.attractiveForce.maxRange )
				{
					force = 0.0f;
				}
				else if( targetDistance > m_pEffectNode->LocationAbs.attractiveForce.minRange )
				{
					force *= 1.0f - (targetDistance - minRange) / (maxRange - minRange);
				}
			}

			m_GlobalRevisionVelocity += targetDirection * force * deltaFrame;
			float currentVelocity = Vector3D::Length( m_GlobalRevisionVelocity );
			Vector3D currentDirection = m_GlobalRevisionVelocity / currentVelocity;
		
			m_GlobalRevisionVelocity = (targetDirection * control + currentDirection * (1.0f - control)) * currentVelocity;
			m_GlobalRevisionLocation += m_GlobalRevisionVelocity * deltaFrame;
		}
	}

	Matrix43 MatTraGlobal;
	MatTraGlobal.Translation( m_GlobalRevisionLocation.X, m_GlobalRevisionLocation.Y, m_GlobalRevisionLocation.Z );
	Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, MatTraGlobal );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Draw()
{
	assert( m_pEffectNode != NULL );

	if( !m_pEffectNode->IsRendered ) return;

	if( m_sequenceNumber != ((ManagerImplemented*)m_pManager)->GetSequenceNumber() )
	{
		CalculateMatrix( 0 );
	}

	m_pEffectNode->Rendering(*this, m_pManager);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Kill()
{
	if( m_State == INSTANCE_STATE_ACTIVE )
	{
		for( InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByInstance )
		{
			group->IsReferencedFromInstance = false;
		}

		m_State = INSTANCE_STATE_REMOVING;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RectF Instance::GetUV() const
{
	if( m_pEffectNode->Texture.UVType == ParameterTexture::UV_DEFAULT )
	{
		return RectF( 0.0f, 0.0f, 1.0f, 1.0f );
	}
	else if( m_pEffectNode->Texture.UVType == ParameterTexture::UV_FIXED )
	{
		return RectF(
			m_pEffectNode->Texture.UV.Fixed.Position.x,
			m_pEffectNode->Texture.UV.Fixed.Position.y,
			m_pEffectNode->Texture.UV.Fixed.Position.w,
			m_pEffectNode->Texture.UV.Fixed.Position.h );
	}
	else if( m_pEffectNode->Texture.UVType == ParameterTexture::UV_ANIMATION )
	{
		int32_t frameNum = (int32_t)(m_LivingTime / m_pEffectNode->Texture.UV.Animation.FrameLength);
		int32_t frameCount = m_pEffectNode->Texture.UV.Animation.FrameCountX * m_pEffectNode->Texture.UV.Animation.FrameCountY;

		if( m_pEffectNode->Texture.UV.Animation.LoopType == m_pEffectNode->Texture.UV.Animation.LOOPTYPE_ONCE )
		{
			if( frameNum >= frameCount )
			{
				frameNum = frameCount - 1;
			}
		}
		else if ( m_pEffectNode->Texture.UV.Animation.LoopType == m_pEffectNode->Texture.UV.Animation.LOOPTYPE_LOOP )
		{
			frameNum %= frameCount;
		}
		else if ( m_pEffectNode->Texture.UV.Animation.LoopType == m_pEffectNode->Texture.UV.Animation.LOOPTYPE_REVERSELOOP )
		{
			bool rev = (frameNum / frameCount) % 2 == 1;
			frameNum %= frameCount;
			if( rev )
			{
				frameNum = frameCount - 1 - frameNum;
			}
		}

		int32_t frameX = frameNum % m_pEffectNode->Texture.UV.Animation.FrameCountX;
		int32_t frameY = frameNum / m_pEffectNode->Texture.UV.Animation.FrameCountX;

		return RectF(
			m_pEffectNode->Texture.UV.Animation.Position.x + m_pEffectNode->Texture.UV.Animation.Position.w * frameX,
			m_pEffectNode->Texture.UV.Animation.Position.y + m_pEffectNode->Texture.UV.Animation.Position.h * frameY,
			m_pEffectNode->Texture.UV.Animation.Position.w,
			m_pEffectNode->Texture.UV.Animation.Position.h );
	}
	else if( m_pEffectNode->Texture.UVType == ParameterTexture::UV_SCROLL )
	{
		return RectF(
			m_pEffectNode->Texture.UV.Scroll.Position.x + m_pEffectNode->Texture.UV.Scroll.Speed.x * m_LivingTime,
			m_pEffectNode->Texture.UV.Scroll.Position.y + m_pEffectNode->Texture.UV.Scroll.Speed.y * m_LivingTime,
			m_pEffectNode->Texture.UV.Scroll.Position.w,
			m_pEffectNode->Texture.UV.Scroll.Position.h );
	}

	return RectF( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGlobal::InstanceGlobal()
	: m_instanceCount	( 0 )
	, m_updatedFrame	( 0 )
	, m_rootContainer	( NULL )
{
	
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGlobal::~InstanceGlobal()
{
	
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGlobal::AddUpdatedFrame( float frame )
{
	m_updatedFrame += frame;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGlobal::IncInstanceCount()
{
	m_instanceCount++;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGlobal::DecInstanceCount()
{
	m_instanceCount--;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int InstanceGlobal::GetInstanceCount()
{
	return m_instanceCount;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float InstanceGlobal::GetUpdatedFrame()
{
	return m_updatedFrame;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer* InstanceGlobal::GetRootContainer() const
{
	return m_rootContainer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGlobal::SetRootContainer( InstanceContainer* container )
{
	m_rootContainer = container;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const Vector3D& InstanceGlobal::GetTargetLocation() const
{
	return m_targetLocation;
}

void InstanceGlobal::SetTargetLocation( const Vector3D& location )
{
	m_targetLocation = location;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGroup::InstanceGroup( Manager* manager, EffectNode* effectNode, InstanceContainer* container, InstanceGlobal* global )
	: m_manager		( (ManagerImplemented*)manager )
	, m_effectNode((EffectNodeImplemented*) effectNode)
	, m_container	( container )
	, m_global		( global )
	, m_time		( 0 )
	, IsReferencedFromInstance	( true )
	, NextUsedByInstance	( NULL )
	, NextUsedByContainer	( NULL )
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceGroup::~InstanceGroup()
{
	RemoveForcibly();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGroup::RemoveInvalidInstances()
{
	std::list<Instance*>::iterator it = m_removingInstances.begin();

	while( it != m_removingInstances.end() )
	{
		if( (*it)->m_State == INSTANCE_STATE_ACTIVE )
		{
			assert(0);
		}
		else if( (*it)->m_State == INSTANCE_STATE_REMOVING )
		{
			// 削除中処理
			(*it)->m_State = INSTANCE_STATE_REMOVED;
			it++;
		}
		else if( (*it)->m_State == INSTANCE_STATE_REMOVED )
		{
			// 削除処理
			if( (*it)->m_pEffectNode->GetType() == EFFECT_NODE_TYPE_ROOT )
			{
				delete (*it);
			}
			else
			{
				(*it)->~Instance();
				m_manager->PushInstance( (*it) );
			}

			it = m_removingInstances.erase( it );
			m_global->DecInstanceCount();
		}
	}
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Instance* InstanceGroup::CreateInstance()
{
	Instance* instance = NULL;

	if( m_effectNode->GetType() == EFFECT_NODE_TYPE_ROOT )
	{
		instance = new Instance( m_manager, m_effectNode, m_container );
	}
	else
	{
		Instance* buf = m_manager->PopInstance();
		if( buf == NULL ) return NULL;
		instance = new(buf)Instance( m_manager, m_effectNode, m_container );
	}
	
	m_instances.push_back( instance );
	m_global->IncInstanceCount();
	return instance;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Instance* InstanceGroup::GetFirst()
{
	if( m_instances.size() > 0 )
	{
		return m_instances.front();
	}
	return NULL;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int InstanceGroup::GetInstanceCount() const
{
	return (int32_t)m_instances.size();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int InstanceGroup::GetRemovingInstanceCount() const
{
	return (int32_t)m_removingInstances.size();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGroup::Update( float deltaFrame, bool shown )
{
	RemoveInvalidInstances();

	std::list<Instance*>::iterator it = m_instances.begin();

	while( it != m_instances.end() )
	{
		if( (*it)->m_State == INSTANCE_STATE_ACTIVE )
		{
			// 更新処理
			(*it)->Update( deltaFrame, shown );

			// 破棄チェック
			if( (*it)->m_State != INSTANCE_STATE_ACTIVE )
			{
				m_removingInstances.push_back( (*it) );
				it = m_instances.erase( it );
			}
			else
			{
				it++;
			}
		}
	}

	m_time++;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGroup::SetBaseMatrix( const Matrix43& mat )
{
	for (auto& instance : m_instances)
	{
		if (instance->m_State == INSTANCE_STATE_ACTIVE)
		{
			Matrix43::Multiple(instance->m_GlobalMatrix43, instance->m_GlobalMatrix43, mat);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGroup::RemoveForcibly()
{
	KillAllInstances();

	RemoveInvalidInstances();
	RemoveInvalidInstances();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceGroup::KillAllInstances()
{
	for (auto& instance : m_instances)
	{
		if (instance->GetState() == INSTANCE_STATE_ACTIVE)
		{
			instance->Kill();
			m_removingInstances.push_back(instance);
		}
	}

	m_instances.clear();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Setting::Setting()
	: m_coordinateSystem(CoordinateSystem::RH)
	, m_effectLoader(NULL)
	, m_textureLoader(NULL)
	, m_soundLoader(NULL)
	, m_modelLoader(NULL)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Setting::~Setting()
{
	ES_SAFE_DELETE(m_effectLoader);
	ES_SAFE_DELETE(m_textureLoader);
	ES_SAFE_DELETE(m_soundLoader);
	ES_SAFE_DELETE(m_modelLoader);

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Setting* Setting::Create()
{
	return new Setting();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
CoordinateSystem Setting::GetCoordinateSystem() const
{
	return m_coordinateSystem;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Setting::SetCoordinateSystem(CoordinateSystem coordinateSystem)
{
	m_coordinateSystem = coordinateSystem;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectLoader* Setting::GetEffectLoader()
{
	return m_effectLoader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Setting::SetEffectLoader(EffectLoader* loader)
{
	ES_SAFE_DELETE(m_effectLoader);
	m_effectLoader = loader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
TextureLoader* Setting::GetTextureLoader()
{
	return m_textureLoader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Setting::SetTextureLoader(TextureLoader* loader)
{
	ES_SAFE_DELETE(m_textureLoader);
	m_textureLoader = loader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelLoader* Setting::GetModelLoader()
{
	return m_modelLoader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Setting::SetModelLoader(ModelLoader* loader)
{
	ES_SAFE_DELETE(m_modelLoader);
	m_modelLoader = loader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundLoader* Setting::GetSoundLoader()
{
	return m_soundLoader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Setting::SetSoundLoader(SoundLoader* loader)
{
	ES_SAFE_DELETE(m_soundLoader);
	m_soundLoader = loader;
}




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------﻿
#ifndef	__EFFEKSEER_SOCKET_H__
#define	__EFFEKSEER_SOCKET_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#ifdef _WIN32
#else
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
typedef signed char			int8_t;
typedef unsigned char		uint8_t;
typedef short				int16_t;
typedef unsigned short		uint16_t;
typedef int					int32_t;
typedef unsigned int		uint32_t;
typedef __int64				int64_t;
typedef unsigned __int64	uint64_t;
#else
#endif
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#ifdef _WIN32

typedef SOCKET	EfkSocket;
typedef int		SOCKLEN;
const EfkSocket InvalidSocket = INVALID_SOCKET;
const int32_t SocketError = SOCKET_ERROR;
const int32_t InaddrNone = INADDR_NONE;

#else

typedef int32_t	EfkSocket;
typedef socklen_t SOCKLEN;
const EfkSocket InvalidSocket = -1;
const int32_t SocketError = -1;
const int32_t InaddrNone = -1;

typedef struct hostent HOSTENT;
typedef struct in_addr IN_ADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#endif

#ifdef _WIN32
static void Sleep_(int32_t ms)
{
	Sleep(ms);
}
#else
static void Sleep_(int32_t ms)
{
	usleep(1000 * ms);
}
#endif


class Socket
{
private:

public:
	static void Initialize();
	static void Finalize();

	static EfkSocket GenSocket();

	static void Close( EfkSocket s );
	static void Shutsown( EfkSocket s );

	static bool Listen( EfkSocket s, int32_t backlog );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_SOCKET_H__
﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#ifdef _WIN32
#pragma comment( lib, "ws2_32.lib" )
#else
#endif


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Socket::Initialize()
{
#ifdef _WIN32
	/* Winsock初期化 */
	WSADATA m_WsaData;
	::WSAStartup( MAKEWORD(2,0), &m_WsaData );
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Socket::Finalize()
{
#ifdef _WIN32
	/* Winsock参照カウンタ減少+破棄 */
	WSACleanup();
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EfkSocket Socket::GenSocket()
{
	return ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Socket::Close( EfkSocket s )
{
#ifdef _WIN32
	::closesocket( s );
#else
	::close( s );
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Socket::Shutsown( EfkSocket s )
{
#ifdef _WIN32
	::shutdown( s, SD_BOTH );
#else
	::shutdown( s, SHUT_RDWR );
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool Socket::Listen( EfkSocket s, int32_t backlog )
{
#ifdef _WIN32
	return ::listen( s, backlog ) != SocketError;
#else
	return listen( s, backlog ) >= 0;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

﻿
#ifndef	__EFFEKSEER_SERVER_IMPLEMENTED_H__
#define	__EFFEKSEER_SERVER_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class ServerImplemented : public Server
{
private:
	class InternalClient
	{
	public:
		Thread		m_threadRecv;
		EfkSocket	m_socket;
		ServerImplemented*		m_server;
		bool		m_active;

		std::vector<uint8_t>	m_recvBuffer;

		std::vector<std::vector<uint8_t> >	m_recvBuffers;
		CriticalSection						m_ctrlRecvBuffers;

		static void RecvAsync( void* data );

	public:
		InternalClient( EfkSocket socket_, ServerImplemented* server );
		~InternalClient();
		void ShutDown();
	};

private:
	EfkSocket	m_socket;
	uint16_t	m_port;

	Thread		m_thread;
	CriticalSection		m_ctrlClients;

	bool		m_running;

	std::set<InternalClient*>	m_clients;
	std::set<InternalClient*>	m_removedClients;

	std::map<std::wstring,Effect*>	m_effects;

	std::map<std::wstring,std::vector<uint8_t> >	m_data;

	std::vector<EFK_CHAR>	m_materialPath;

	void AddClient( InternalClient* client );
	void RemoveClient( InternalClient* client );
	static void AcceptAsync( void* data );

public:

	ServerImplemented();
	virtual ~ServerImplemented();

	/**
		@brief	サーバーを開始する。
	*/
	bool Start( uint16_t port );

	void Stop();

	void Regist( const EFK_CHAR* key, Effect* effect );

	void Unregist( Effect* effect );

	void Update();

	void SetMaterialPath( const EFK_CHAR* materialPath );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_SERVER_IMPLEMENTED_H__
﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#ifdef _WIN32
#else
#endif


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::InternalClient::RecvAsync( void* data )
{
	InternalClient* client = (InternalClient*)data;

	while(true)
	{
		client->m_recvBuffer.clear();

		int32_t size = 0;
		int32_t restSize = 0;

		restSize = 4;
		while(restSize > 0)
		{
			int32_t recvSize = ::recv( client->m_socket, (char*)(&size), restSize, 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				/* 失敗 */
				client->m_server->RemoveClient( client );
				client->ShutDown();
				return;
			}
		}

		restSize = size;
		while(restSize > 0)
		{
			uint8_t buf[256];

			int32_t recvSize = ::recv( client->m_socket, (char*)(buf), Min(restSize,256), 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				/* 失敗 */
				client->m_server->RemoveClient( client );
				client->ShutDown();
				return;
			}

			for( int32_t i = 0; i < recvSize; i++ )
			{
				client->m_recvBuffer.push_back( buf[i] );
			}
		}

		/* 受信処理 */
		client->m_ctrlRecvBuffers.Enter();
		client->m_recvBuffers.push_back(client->m_recvBuffer);
		client->m_ctrlRecvBuffers.Leave();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ServerImplemented::InternalClient::InternalClient( EfkSocket socket_, ServerImplemented* server )
	: m_socket	( socket_ )
	, m_server	( server )
	, m_active	( true )
{
	m_threadRecv.Create( RecvAsync, this );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ServerImplemented::InternalClient::~InternalClient()
{
	m_threadRecv.Wait();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::InternalClient::ShutDown()
{
	if ( m_socket != InvalidSocket )
	{
		Socket::Shutsown( m_socket );
		Socket::Close( m_socket );
		m_socket = InvalidSocket;
		m_active = false;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ServerImplemented::ServerImplemented()
	: m_running		( false )
{
	Socket::Initialize();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ServerImplemented::~ServerImplemented()
{
	Stop();

	Socket::Finalize();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Server* Server::Create()
{
	return new ServerImplemented();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::AddClient( InternalClient* client )
{
	m_ctrlClients.Enter();
	m_clients.insert( client );
	m_ctrlClients.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::RemoveClient( InternalClient* client )
{
	m_ctrlClients.Enter();
	if( m_clients.count( client ) > 0 )
	{
		m_clients.erase( client );
		m_removedClients.insert( client );
	}
	m_ctrlClients.Leave();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::AcceptAsync( void* data )
{
	ServerImplemented* server = (ServerImplemented*)data;

	while(true)
	{
		SOCKADDR_IN socketAddrIn;
		int32_t Size = sizeof(SOCKADDR_IN);

		EfkSocket socket_ = ::accept( 
			server->m_socket, 
			(SOCKADDR*)(&socketAddrIn),
			(SOCKLEN*)(&Size) );

		if ( server->m_socket == InvalidSocket || socket_ == InvalidSocket )
		{
			break;
		}

		/* 接続追加 */
		server->AddClient( new InternalClient( socket_, server ) );

		EffekseerPrintDebug("Server : AcceptClient\n");

	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ServerImplemented::Start( uint16_t port )
{
	if( m_running )
	{
		Stop();
	}

	int32_t returnCode;
	sockaddr_in sockAddr = { AF_INET };

	/* ソケット生成 */
	EfkSocket socket_ = Socket::GenSocket();
	if ( socket_ == InvalidSocket )
	{
		return false;
	}

	/* 接続用データ生成 */
	memset( &sockAddr, 0, sizeof(SOCKADDR_IN));
	sockAddr.sin_family	= AF_INET;
	sockAddr.sin_port	= htons( port );

	/* 関連付け */
	returnCode = ::bind( socket_, (sockaddr*)&sockAddr, sizeof(sockaddr_in) );
	if ( returnCode == SocketError )
	{
		if ( socket_ != InvalidSocket )
		{
			Socket::Close( socket_ );
		}
		return false;
	}

	/* 接続 */
	if ( !Socket::Listen( socket_, 30 ) )
	{
		if ( socket_ != InvalidSocket )
		{
			Socket::Close( socket_ );
		}
		return false;
	}

	m_running = true;
	m_socket = socket_;
	m_port = port;
	m_thread.Create( AcceptAsync, this );

	EffekseerPrintDebug("Server : Start\n");

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Stop()
{
	if( !m_running ) return;

	Socket::Shutsown( m_socket );
	Socket::Close( m_socket );
	m_socket = InvalidSocket;
	
	m_running = false;

	m_thread.Wait();

	/* クライアント停止 */
	m_ctrlClients.Enter();
	for( std::set<InternalClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it )
	{
		(*it)->ShutDown();
	}
	m_ctrlClients.Leave();
	

	/* クライアントの消滅待ち */
	while(true)
	{
		m_ctrlClients.Enter();
		int32_t size = m_clients.size();
		m_ctrlClients.Leave();
	
		if( size == 0 ) break;

		Sleep_(1);
	}

	/* 破棄 */
	for( std::set<InternalClient*>::iterator it = m_removedClients.begin(); it != m_removedClients.end(); ++it )
	{
		while( (*it)->m_active )
		{
			Sleep_(1);
		}
		delete (*it);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Regist( const EFK_CHAR* key, Effect* effect )
{
	if( effect == NULL ) return;

	std::wstring key_( (const wchar_t*)key );

	if( m_effects.count( key_ ) > 0 )
	{
		m_effects[key_]->Release();
	}

	m_effects[key_] = effect;
	m_effects[key_]->AddRef();

	if( m_data.count( key_ ) > 0 )
	{
		if( m_materialPath.size() > 1 )
		{
			m_effects[key_]->Reload( &(m_data[key_][0]), m_data.size(), &(m_materialPath[0]) );
		}
		else
		{
			m_effects[key_]->Reload( &(m_data[key_][0]), m_data.size() );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Unregist( Effect* effect )
{
	if( effect == NULL ) return;

	std::map<std::wstring,Effect*>::iterator it = m_effects.begin();
	std::map<std::wstring,Effect*>::iterator it_end = m_effects.end();

	while( it != it_end )
	{
		if( (*it).second == effect )
		{
			(*it).second->Release();
			m_effects.erase( it );
			return;
		}

		it++;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Update()
{
	m_ctrlClients.Enter();

	for( std::set<InternalClient*>::iterator it = m_removedClients.begin(); it != m_removedClients.end(); ++it )
	{
		while( (*it)->m_active )
		{
			Sleep_(1);
		}
		delete (*it);
	}
	m_removedClients.clear();

	for( std::set<InternalClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it )
	{
		(*it)->m_ctrlRecvBuffers.Enter();

		for( size_t i = 0; i < (*it)->m_recvBuffers.size(); i++ )
		{
			std::vector<uint8_t>& buf = (*it)->m_recvBuffers[i];

			uint8_t* p = &(buf[0]);
		
			int32_t keylen = 0;
			memcpy( &keylen, p, sizeof(int32_t) );
			p += sizeof(int32_t);

			std::wstring key;
			for( int32_t k = 0; k < keylen; k++ )
			{
				key.push_back( ((wchar_t*)p)[0] );
				p += sizeof(wchar_t);
			}

			uint8_t* data = p;
			int32_t datasize = buf.size() - (p-&(buf[0]));
		
			if( m_data.count( key ) > 0 )
			{
				m_data[key].clear();
			}

			for( int32_t d = 0; d < datasize; d++ )
			{
				m_data[key].push_back( data[d] );
			}

			if( m_effects.count( key ) > 0 )
			{
				if( m_materialPath.size() > 1 )
				{
					m_effects[key]->Reload( &(m_data[key][0]), m_data.size(), &(m_materialPath[0]) );
				}
				else
				{
					m_effects[key]->Reload( &(m_data[key][0]), m_data.size() );
				}
			}
		}

		(*it)->m_recvBuffers.clear();
		(*it)->m_ctrlRecvBuffers.Leave();

	}
	m_ctrlClients.Leave();
	
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::SetMaterialPath( const EFK_CHAR* materialPath )
{
	m_materialPath.clear();

	int32_t pos = 0;
	while( materialPath[pos] != 0 )
	{
		m_materialPath.push_back( materialPath[pos] );
		pos++;
	}
	m_materialPath.push_back(0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
﻿
#ifndef	__EFFEKSEER_CLIENT_IMPLEMENTED_H__
#define	__EFFEKSEER_CLIENT_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class ClientImplemented : public Client
{
private:
	Thread		m_threadRecv;

	EfkSocket	m_socket;
	uint16_t	m_port;
	std::vector<uint8_t>	m_sendBuffer;

	bool		m_running;

	HOSTENT* GetHostEntry( const char* host );

	static void RecvAsync( void* data );
public:
	ClientImplemented();
	~ClientImplemented();

	bool Start( char* host, uint16_t port );
	void Stop();

	bool Send( void* data, int32_t datasize );

	void Reload( const EFK_CHAR* key, void* data, int32_t size );
	void Reload( Manager* manager, const EFK_CHAR* path, const EFK_CHAR* key );

	bool IsConnected();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_CLIENT_IMPLEMENTED_H__
﻿
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ClientImplemented::RecvAsync( void* data )
{
	ClientImplemented* client = (ClientImplemented*)data;

	while(true)
	{
		int32_t size = 0;
		int32_t restSize = 0;

		restSize = 4;
		while(restSize > 0)
		{
			int32_t recvSize = ::recv( client->m_socket, (char*)(&size), restSize, 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				/* 失敗 */
				client->Stop();
				return;
			}
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ClientImplemented::ClientImplemented()
	: m_running		( false )
{
	Socket::Initialize();
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ClientImplemented::~ClientImplemented()
{
	Stop();

	Socket::Finalize();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Client* Client::Create()
{
	return new ClientImplemented();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
HOSTENT* ClientImplemented::GetHostEntry( const char* host )
{
	HOSTENT* hostEntry = NULL;
	IN_ADDR InAddrHost;

	/* IPアドレスかDNSか調べる */
	InAddrHost.s_addr = ::inet_addr( host );
	if ( InAddrHost.s_addr == InaddrNone )
	{
		/* DNS */
		hostEntry = ::gethostbyname( host );
		if ( hostEntry == NULL )
		{
			return NULL;
		}
	}
	else
	{
		/* IPアドレス */
		hostEntry = ::gethostbyaddr( (const char*)(&InAddrHost), sizeof(IN_ADDR), AF_INET );
		if ( hostEntry == NULL )
		{
			return NULL;
		}
	}

	return hostEntry;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ClientImplemented::Start( char* host, uint16_t port )
{
	if( m_running ) return false;

	SOCKADDR_IN sockAddr;
	HOSTENT* hostEntry= NULL;
	
	/* ソケット生成 */
	EfkSocket socket_ = Socket::GenSocket();
	if ( socket_ == InvalidSocket )
	{
		if ( socket_ != InvalidSocket ) Socket::Close( socket_ );
		return false;
	}

	/* ホスト情報取得 */
	hostEntry = GetHostEntry( host );
	if ( hostEntry == NULL )
	{
		if ( socket_ != InvalidSocket ) Socket::Close( socket_ );
		return false;
	}

	/* 接続用データ生成 */
	memset( &sockAddr, 0, sizeof(SOCKADDR_IN) );
	sockAddr.sin_family	= AF_INET;
	sockAddr.sin_port	= htons( port );
	sockAddr.sin_addr	= *(IN_ADDR*)(hostEntry->h_addr_list[0]);

	/* 接続 */
	int32_t ret = ::connect( socket_, (SOCKADDR*)(&sockAddr), sizeof(SOCKADDR_IN) );
	if ( ret == SocketError )
	{
		if ( socket_ != InvalidSocket ) Socket::Close( socket_ );
		return false;
	}

	m_socket = socket_;
	m_port = port;

	m_running = true;

	m_threadRecv.Create( RecvAsync, this );

	EffekseerPrintDebug("Client : Start\n");

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ClientImplemented::Stop()
{
	if( !m_running ) return;

	Socket::Shutsown( m_socket );
	Socket::Close( m_socket );
	m_running = false;

	EffekseerPrintDebug("Client : Stop\n");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ClientImplemented::Send( void* data, int32_t datasize )
{
	if( !m_running ) return false;

	m_sendBuffer.clear();
	for( int32_t i = 0; i < sizeof(int32_t); i++ )
	{
		m_sendBuffer.push_back( ((uint8_t*)(&datasize))[i] );
	}

	for( int32_t i = 0; i < datasize; i++ )
	{
		m_sendBuffer.push_back( ((uint8_t*)(data))[i] );
	}

	int32_t size = m_sendBuffer.size();
	while( size > 0 )
	{
		int32_t ret = ::send( m_socket, (const char*)(&(m_sendBuffer[m_sendBuffer.size()-size])), size, 0 );
		if( ret == 0 || ret < 0 )
		{
			Stop();
			return false;
		}
		size -= ret;
	}

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ClientImplemented::Reload( const EFK_CHAR* key, void* data, int32_t size )
{
	int32_t keylen = 0;
	for( ; ; keylen++ )
	{
		if(key[keylen] == 0 ) break;
	}

	std::vector<uint8_t> buf;

	for( int32_t i = 0; i < sizeof(int32_t); i++ )
	{
		buf.push_back( ((uint8_t*)(&keylen))[i] );
	}

	for( int32_t i = 0; i < keylen * 2; i++ )
	{
		buf.push_back( ((uint8_t*)(key))[i] );
	}

	for( int32_t i = 0; i < size; i++ )
	{
		buf.push_back( ((uint8_t*)(data))[i] );
	}

	Send( &(buf[0]), buf.size() );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ClientImplemented::Reload( Manager* manager, const EFK_CHAR* path, const EFK_CHAR* key )
{
	EffectLoader* loader = manager->GetEffectLoader();
	
	void* data = NULL;
	int32_t size = 0;

	if( !loader->Load( path, data, size ) ) return;

	Reload( key, data, size );

	loader->Unload( data, size );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ClientImplemented::IsConnected()
{
	return m_running;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
