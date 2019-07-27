#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#define NOMINMAX
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
#include <limits>
#include <complex>
#include <stdlib.h>
#include <cstdlib>
#include <random>
#include <thread>
#include <mutex>
#include <iostream>
#include <array>
#ifdef _WIN32
#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#if (_M_IX86_FP >= 2) || defined(__SSE__)
#define EFK_SSE2
#include <emmintrin.h>
#elif defined(__ARM_NEON__)
#define EFK_NEON
#include <arm_neon.h>
#endif

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
typedef char16_t			EFK_CHAR;

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
class EffectNodeImplemented;
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
#endif	// __EFFEKSEER_BASE_H__
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
			return sqrtf(GetSquaredLength());
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

#endif




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
}




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
}



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
				if (Type == OBJECT_SHAPE_TYPE_CUBOID) radius = sqrtf(Data.Cuboid.X * Data.Cuboid.X + Data.Cuboid.Y * Data.Cuboid.Y + Data.Cuboid.Z * Data.Cuboid.Z) / 2.0f;
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
		void SetUserData(void* userData_) override;

		void SetWorld(World* world_);

		Status	GetCurrentStatus() { return currentStatus; }
		Status	GetNextStatus() { return nextStatus; }

		int32_t ObjectIndex;

		virtual int32_t GetRef() override { return ReferenceObject::GetRef(); }
		virtual int32_t AddRef() override { return ReferenceObject::AddRef(); }
		virtual int32_t Release() override { return ReferenceObject::Release(); }
	};
}






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

		void AddObject(Object* o) override;
		void RemoveObject(Object* o) override;

		void AddObjectInternal(Object* o);
		void RemoveObjectInternal(Object* o);

		void CastRay(Vector3DF from, Vector3DF to) override;

		void Culling(const Matrix44& cameraProjMat, bool isOpenGL) override;

		bool Reassign() override;

		void Dump(const char* path, const Matrix44& cameraProjMat, bool isOpenGL) override;

		int32_t GetObjectCount() override { return (int32_t)objs.size(); }
		Object* GetObject(int32_t index) override { return objs[index]; }

		virtual int32_t GetRef() override { return ReferenceObject::GetRef(); }
		virtual int32_t AddRef() override { return ReferenceObject::AddRef(); }
		virtual int32_t Release() override { return ReferenceObject::Release(); }
	};
}


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
		o_->ObjectIndex = (int32_t)objects.size() - 1;
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
}


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
}

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
}

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

	void ObjectInternal::SetUserData(void* userData_)
	{
		this->userData = userData_;
	}


	void ObjectInternal::SetWorld(World* world_)
	{
		this->world = world_;
	}
}

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
}

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
		return sqrtf(dx * dx + dy * dy + dz * dz);
	}
}


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

					for (int k = 0; k < 3; ++k)
					{
						if (std::abs(d[k]) < FLT_EPSILON)
						{
							if (p[k] < min[k] || p[k] > max[k])
							{
								// 交差していない
								continue;
							}
						}
						else
						{
							// スラブとの距離を算出
							// t1が近スラブ、t2が遠スラブとの距離
							float odd = 1.0f / d[k];
							float t1 = (min[k] - p[k]) * odd;
							float t2 = (max[k] - p[k]) * odd;
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

	void WorldInternal::Culling(const Matrix44& cameraProjMat, bool isOpenGL)
	{
		objs.clear();
	

		if (!std::isinf(cameraProjMat.Values[2][2]) &&
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
							float x_ = 0.0f, y_ = 0.0f, z_ = 0.0f;
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

		auto xlen = Max(std::abs(xmax), std::abs(xmin)) * 2.0f;
		auto ylen = Max(std::abs(ymax), std::abs(ymin)) * 2.0f;
		auto zlen = Max(std::abs(zmax), std::abs(zmin)) * 2.0f;

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
						float x_ = 0.0f, y_ = 0.0f, z_ = 0.0f;
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
}


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
//----------------------------------------------------------------------------------

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

Vector3D Vector3D::operator-()
{
	return Vector3D(-X, -Y, -Z);
}

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

Vector3D Vector3D::operator * (const Vector3D& o) const
{
	return Vector3D(X * o.X, Y * o.Y, Z * o.Z);
}

Vector3D Vector3D::operator / (const Vector3D& o) const
{
	return Vector3D(X / o.X, Y / o.Y, Z / o.Z);
}

bool Vector3D::operator == (const Vector3D& o)
{
	return this->X == o.X && this->Y == o.Y && this->Z == o.Z;
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
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(__SSE__)
#define EFK_SSE2
#elif defined(__ARM_NEON__)
#define EFK_NEON
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

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
Color Color::Mul( Color in1, Color in2 )
{
#if defined(EFK_SSE2)
	__m128i s1 = _mm_cvtsi32_si128(*(int32_t*)&in1);
	__m128i s2 = _mm_cvtsi32_si128(*(int32_t*)&in2);
	__m128i zero = _mm_setzero_si128();
	__m128i mask = _mm_set1_epi16(1);

	s1 = _mm_unpacklo_epi8(s1, zero);
	s2 = _mm_unpacklo_epi8(s2, zero);

	__m128i r0 = _mm_mullo_epi16(s1, s2);
	__m128i r1 = _mm_srli_epi16(r0, 8);
	__m128i r2 = _mm_and_si128(r0, mask);
	__m128i r3 = _mm_or_si128(r2, r1);
	__m128i res = _mm_packus_epi16(r3, zero);
	
	Color o;
	*(int*)&o = _mm_cvtsi128_si32(res);
	return o;
#elif defined(EFK_NEON)
	uint8x8_t s1 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in1));
	uint8x8_t s2 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in2));
	uint16x8_t mask = vmovq_n_u16(1);
	uint16x8_t s3 = vmovl_u8(s1);
	uint16x8_t s4 = vmovl_u8(s2);
	uint16x8_t r0 = vmulq_u16(s3, s4);
	uint16x8_t r1 = vshrq_n_u16(r0, 8);
	uint16x8_t r2 = vandq_u16(r0, mask);
	uint16x8_t r3 = vorrq_u16(r2, r1);
	uint8x8_t res = vqmovn_u16(r3);
	
	Color o;
	*(uint32_t*)&o = vget_lane_u32(vreinterpret_u32_u8(res), 0);
	return o;
#else
	Color o;
	o.R = (uint8_t)((float)in1.R * (float)in2.R / 255.0f);
	o.G = (uint8_t)((float)in1.G * (float)in2.G / 255.0f);
	o.B = (uint8_t)((float)in1.B * (float)in2.B / 255.0f);
	o.A = (uint8_t)((float)in1.A * (float)in2.A / 255.0f);
	return o;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Color Color::Mul( Color in1, float in2 )
{
#if defined(EFK_SSE2)
	__m128i s1 = _mm_cvtsi32_si128(*(int32_t*)&in1);
	__m128i s2 = _mm_set1_epi16((int16_t)(in2 * 256));
	__m128i zero = _mm_setzero_si128();
	
	s1 = _mm_unpacklo_epi8(s1, zero);

	__m128i res = _mm_mullo_epi16(s1, s2);
	res = _mm_srli_epi16(res, 8);
	res = _mm_packus_epi16(res, zero);
	
	Color o;
	*(int*)&o = _mm_cvtsi128_si32(res);
	return o;
#elif defined(EFK_NEON)
	uint8x8_t s1 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in1));
	uint16x8_t s2 = vmovq_n_u16((uint16_t)(in2 * 256));
	uint16x8_t s3 = vmovl_u8(s1);
	uint16x8_t r0 = vmulq_u16(s3, s2);
	uint16x8_t r1 = vshrq_n_u16(r0, 8);
	uint8x8_t res = vqmovn_u16(r1);
	
	Color o;
	*(uint32_t*)&o = vget_lane_u32(vreinterpret_u32_u8(res), 0);
	return o;
#else
	Color o;
	o.R = (uint8_t)Clamp((int)((float)in1.R * in2), 255, 0);
	o.G = (uint8_t)Clamp((int)((float)in1.G * in2), 255, 0);
	o.B = (uint8_t)Clamp((int)((float)in1.B * in2), 255, 0);
	o.A = (uint8_t)Clamp((int)((float)in1.A * in2), 255, 0);
	return o;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Color Color::Lerp( const Color in1, const Color in2, float t )
{
	/*
#if defined(EFK_SSE2)
	__m128i s1 = _mm_cvtsi32_si128(*(int32_t*)&in1);
	__m128i s2 = _mm_cvtsi32_si128(*(int32_t*)&in2);
	__m128i tm = _mm_set1_epi16((int16_t)(t * 256));
	__m128i zero = _mm_setzero_si128();

	s1 = _mm_unpacklo_epi8(s1, zero);
	s2 = _mm_unpacklo_epi8(s2, zero);
	
	__m128i r0 = _mm_subs_epi16(s2, s1);
	__m128i r1 = _mm_mullo_epi16(r0, tm);
	__m128i r2 = _mm_srai_epi16(r1, 8);
	__m128i r3 = _mm_adds_epi16(s1, r2);
	__m128i res = _mm_packus_epi16(r3, zero);
	
	Color o;
	*(int*)&o = _mm_cvtsi128_si32(res);
	return o;
#elif defined(EFK_NEON)

#ifdef __ANDROID__
	uint8x8_t s1 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in1));
	uint8x8_t s2 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in2));
	int16x8_t tm = vmovq_n_s16((int16_t)(t * 256));
	uint16x8_t s3 = vmovl_u8(s1);
	uint16x8_t s4 = vmovl_u8(s2);
	int16x8_t r0 = (int16x8_t)vqsubq_s16((int16x8_t)s4, (int16x8_t)s3);
	int16x8_t r1 = vmulq_s16(r0, tm);
	int16x8_t r2 = vrshrq_n_s16(r1, 8);
	int16x8_t r3 = (int16x8_t)vqaddq_s16((int16x8_t)s3, r2);
	uint8x8_t res = (uint8x8_t)vqmovn_u16((uint16x8_t)r3);

	Color o;
	*(uint32_t*)&o = vget_lane_u32(vreinterpret_u32_u8(res), 0);
	return o;
#else
	uint8x8_t s1 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in1));
	uint8x8_t s2 = vreinterpret_u8_u32(vmov_n_u32(*(uint32_t*)&in2));
	int16x8_t tm = vmovq_n_s16((int16_t)(t * 256));
	uint16x8_t s3 = vmovl_u8(s1);
	uint16x8_t s4 = vmovl_u8(s2);
	int16x8_t r0 = vqsubq_s16(s4, s3);
	int16x8_t r1 = vmulq_s16(r0, tm);
	int16x8_t r2 = vrshrq_n_s16(r1, 8);
	int16x8_t r3 = vqaddq_s16(s3, r2);
	uint8x8_t res = vqmovn_u16(r3);
	
	Color o;
	*(uint32_t*)&o = vget_lane_u32(vreinterpret_u32_u8(res), 0);
	return o;
#endif

#else
	*/
	Color o;
	o.R = (uint8_t)Clamp( in1.R + (in2.R - in1.R) * t, 255, 0 );
	o.G = (uint8_t)Clamp( in1.G + (in2.G - in1.G) * t, 255, 0 );
	o.B = (uint8_t)Clamp( in1.B + (in2.B - in1.B) * t, 255, 0 );
	o.A = (uint8_t)Clamp( in1.A + (in2.A - in1.A) * t, 255, 0 );
	return o;
//#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(__SSE__)
#define EFK_SSE2
#elif defined(__ARM_NEON__)
#define EFK_NEON
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if defined(_MSC_VER)
#define EFK_ALIGN_AS(n) __declspec(align(n))
#else
#define EFK_ALIGN_AS(n) alignas(n)
#endif

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
	
	if( rx != 0.0f )
	{
		::Effekseer::SinCos( rx, sx, cx );
	}
	else
	{
		sx = 0.0f;
		cx = 1.0f;
	}
	if( ry != 0.0f )
	{
		::Effekseer::SinCos( ry, sy, cy );
	}
	else
	{
		sy = 0.0f;
		cy = 1.0f;
	}
	if( rz != 0.0f )
	{
		::Effekseer::SinCos( rz, sz, cz );
	}
	else
	{
		sz = 0.0f;
		cz = 1.0f;
	}

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

	if( rx != 0.0f )
	{
		::Effekseer::SinCos( rx, sx, cx );
	}
	else
	{
		sx = 0.0f;
		cx = 1.0f;
	}
	if( ry != 0.0f )
	{
		::Effekseer::SinCos( ry, sy, cy );
	}
	else
	{
		sy = 0.0f;
		cy = 1.0f;
	}
	if( rz != 0.0f )
	{
		::Effekseer::SinCos( rz, sz, cz );
	}
	else
	{
		sz = 0.0f;
		cz = 1.0f;
	}
	
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
#if defined(EFK_SSE2)
	t.X = Value[3][0];
	t.Y = Value[3][1];
	t.Z = Value[3][2];

	__m128 v0 = _mm_loadu_ps(&Value[0][0]);
	__m128 v1 = _mm_loadu_ps(&Value[1][0]);
	__m128 v2 = _mm_loadu_ps(&Value[2][0]);
	__m128 m0 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 m1 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 s0 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 0, 2, 0));
	__m128 s1 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 1, 3, 1));
	__m128 s2 = _mm_shuffle_ps(m1, v2, _MM_SHUFFLE(0, 2, 2, 0));
	s0 = _mm_mul_ps(s0, s0);
	s1 = _mm_mul_ps(s1, s1);
	s2 = _mm_mul_ps(s2, s2);
	__m128 vscq = _mm_add_ps(_mm_add_ps(s0, s1), s2);
	__m128 vsc = _mm_sqrt_ps(vscq);
	__m128 vscr = _mm_div_ps(vsc, vscq);
	EFK_ALIGN_AS(16) float sc[4];
	_mm_store_ps(sc, vsc);
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
	v0 = _mm_mul_ps(v0, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(0, 0, 0, 0)));
	v1 = _mm_mul_ps(v1, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(1, 1, 1, 1)));
	v2 = _mm_mul_ps(v2, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(2, 2, 2, 2)));
	_mm_storeu_ps(&r.Value[0][0], v0);
	_mm_storeu_ps(&r.Value[1][0], v1);
	_mm_storeu_ps(&r.Value[2][0], v2);
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
#elif defined(EFK_NEON)
	t.X = Value[3][0];
	t.Y = Value[3][1];
	t.Z = Value[3][2];

	float32x4x3_t m = vld3q_f32(&Value[0][0]);
	float32x4_t vscq = vmulq_f32(m.val[0], m.val[0]);
	vscq = vmlaq_f32(vscq, m.val[1], m.val[1]);
	vscq = vmlaq_f32(vscq, m.val[2], m.val[2]);
	float32x4_t scr_rep = vrsqrteq_f32(vscq);
	float32x4_t scr_v = vmulq_f32(vrsqrtsq_f32(vmulq_f32(vscq, scr_rep), scr_rep), scr_rep);
	float32x4_t sc_v = vmulq_f32(scr_v, vscq);
	float sc[4];
	vst1q_f32(sc, sc_v);
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
	float32x4_t v0 = vld1q_f32(&Value[0][0]);
	float32x4_t v1 = vld1q_f32(&Value[1][0]);
	float32x4_t v2 = vld1q_f32(&Value[2][0]);
	vst1q_f32(&r.Value[0][0], vmulq_lane_f32(v0, vget_low_f32(scr_v), 0));
	vst1q_f32(&r.Value[1][0], vmulq_lane_f32(v1, vget_low_f32(scr_v), 1));
	vst1q_f32(&r.Value[2][0], vmulq_lane_f32(v2, vget_high_f32(scr_v), 0));
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
#else
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
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetScale( Vector3D& s ) const
{
#if defined(EFK_SSE2)
	__m128 v0 = _mm_loadu_ps(&Value[0][0]);
	__m128 v1 = _mm_loadu_ps(&Value[1][0]);
	__m128 v2 = _mm_loadu_ps(&Value[2][0]);
	__m128 m0 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 m1 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 s0 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 0, 2, 0));
	__m128 s1 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 1, 3, 1));
	__m128 s2 = _mm_shuffle_ps(m1, v2, _MM_SHUFFLE(0, 2, 2, 0));
	s0 = _mm_mul_ps(s0, s0);
	s1 = _mm_mul_ps(s1, s1);
	s2 = _mm_mul_ps(s2, s2);
	__m128 vscq = _mm_add_ps(_mm_add_ps(s0, s1), s2);
	__m128 sc_v = _mm_sqrt_ps(vscq);
	EFK_ALIGN_AS(16) float sc[4];
	_mm_store_ps(sc, sc_v);
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
#elif defined(EFK_NEON)
	float32x4x3_t m = vld3q_f32(&Value[0][0]);
	float32x4_t vscq = vmulq_f32(m.val[0], m.val[0]);
	vscq = vmlaq_f32(vscq, m.val[1], m.val[1]);
	vscq = vmlaq_f32(vscq, m.val[2], m.val[2]);
	float32x4_t scr_rep = vrsqrteq_f32(vscq);
	float32x4_t scr_v = vmulq_f32(vrsqrtsq_f32(vmulq_f32(vscq, scr_rep), scr_rep), scr_rep);
	float32x4_t sc_v = vmulq_f32(scr_v, vscq);
	float sc[4];
	vst1q_f32(sc, sc_v);
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
#else
	float sc[3];
	for( int m = 0; m < 3; m++ )
	{
		sc[m] = sqrt( Value[m][0] * Value[m][0] + Value[m][1] * Value[m][1] + Value[m][2] * Value[m][2] );
	}
	
	s.X = sc[0];
	s.Y = sc[1];
	s.Z = sc[2];
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::GetRotation( Matrix43& r ) const
{
#if defined(EFK_SSE2)
	__m128 v0 = _mm_loadu_ps(&Value[0][0]);
	__m128 v1 = _mm_loadu_ps(&Value[1][0]);
	__m128 v2 = _mm_loadu_ps(&Value[2][0]);
	__m128 m0 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 m1 = _mm_shuffle_ps(v0, v1, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 s0 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 0, 2, 0));
	__m128 s1 = _mm_shuffle_ps(m0, v2, _MM_SHUFFLE(0, 1, 3, 1));
	__m128 s2 = _mm_shuffle_ps(m1, v2, _MM_SHUFFLE(0, 2, 2, 0));
	s0 = _mm_mul_ps(s0, s0);
	s1 = _mm_mul_ps(s1, s1);
	s2 = _mm_mul_ps(s2, s2);
	__m128 vscq = _mm_add_ps(_mm_add_ps(s0, s1), s2);
	__m128 vsc = _mm_sqrt_ps(vscq);
	__m128 vscr = _mm_div_ps(vsc, vscq);
	v0 = _mm_mul_ps(v0, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(0, 0, 0, 0)));
	v1 = _mm_mul_ps(v1, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(1, 1, 1, 1)));
	v2 = _mm_mul_ps(v2, _mm_shuffle_ps(vscr, vscr, _MM_SHUFFLE(2, 2, 2, 2)));
	_mm_storeu_ps(&r.Value[0][0], v0);
	_mm_storeu_ps(&r.Value[1][0], v1);
	_mm_storeu_ps(&r.Value[2][0], v2);
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
#elif defined(EFK_NEON)
	float32x4x3_t m = vld3q_f32(&Value[0][0]);
	float32x4_t vscq = vmulq_f32(m.val[0], m.val[0]);
	vscq = vmlaq_f32(vscq, m.val[1], m.val[1]);
	vscq = vmlaq_f32(vscq, m.val[2], m.val[2]);
	float32x4_t scr_rep = vrsqrteq_f32(vscq);
	float32x4_t scr_v = vmulq_f32(vrsqrtsq_f32(vmulq_f32(vscq, scr_rep), scr_rep), scr_rep);
	float32x4_t v0 = vld1q_f32(&Value[0][0]);
	float32x4_t v1 = vld1q_f32(&Value[1][0]);
	float32x4_t v2 = vld1q_f32(&Value[2][0]);
	vst1q_f32(&r.Value[0][0], vmulq_lane_f32(v0, vget_low_f32(scr_v), 0));
	vst1q_f32(&r.Value[1][0], vmulq_lane_f32(v1, vget_low_f32(scr_v), 1));
	vst1q_f32(&r.Value[2][0], vmulq_lane_f32(v2, vget_high_f32(scr_v), 0));
	r.Value[3][0] = 0.0f;
	r.Value[3][1] = 0.0f;
	r.Value[3][2] = 0.0f;
#else
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
#endif
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
	Value[0][0] = s.X * r.Value[0][0];
	Value[0][1] = s.X * r.Value[0][1];
	Value[0][2] = s.X * r.Value[0][2];
	Value[1][0] = s.Y * r.Value[1][0];
	Value[1][1] = s.Y * r.Value[1][1];
	Value[1][2] = s.Y * r.Value[1][2];
	Value[2][0] = s.Z * r.Value[2][0];
	Value[2][1] = s.Z * r.Value[2][1];
	Value[2][2] = s.Z * r.Value[2][2];
	Value[3][0] = t.X;
	Value[3][1] = t.Y;
	Value[3][2] = t.Z;
}

void Matrix43::ToMatrix44(Matrix44& dst)
{
	for (int m = 0; m < 4; m++)
	{
		for (int n = 0; n < 3; n++)
		{
			dst.Values[m][n] = Value[m][n];
		}
		dst.Values[m][3] = 0.0f;
	}

	dst.Values[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Matrix43::Multiple( Matrix43& out, const Matrix43& in1, const Matrix43& in2 )
{
#if defined(EFK_SSE2)
	__m128 s1_v0 = _mm_loadu_ps(&in1.Value[0][0]);
	__m128 s1_v1 = _mm_loadu_ps(&in1.Value[1][0]);
	__m128 s1_v2 = _mm_loadu_ps(&in1.Value[2][0]);
	__m128 s1_v3 = _mm_loadu_ps(&in1.Value[3][0] - 1);
	__m128 s2_v0 = _mm_loadu_ps(&in2.Value[0][0]);
	__m128 s2_v1 = _mm_loadu_ps(&in2.Value[1][0]);
	__m128 s2_v2 = _mm_loadu_ps(&in2.Value[2][0]);
	__m128 s2_v3 = _mm_loadu_ps(&in2.Value[3][0] - 1);
	__m128 o_v3;

	{
		__m128 s1_00 = _mm_shuffle_ps(s1_v0, s1_v0, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 s1_01 = _mm_shuffle_ps(s1_v0, s1_v0, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 s1_02 = _mm_shuffle_ps(s1_v0, s1_v0, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 o_m00 = _mm_mul_ps(s1_00, s2_v0);
		__m128 o_m01 = _mm_mul_ps(s1_01, s2_v1);
		__m128 o_m02 = _mm_mul_ps(s1_02, s2_v2);
		__m128 o_v0 = _mm_add_ps(_mm_add_ps(o_m00, o_m01), o_m02);
		_mm_storeu_ps(&out.Value[0][0], o_v0);
	}
	{
		__m128 s1_10 = _mm_shuffle_ps(s1_v1, s1_v1, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 s1_11 = _mm_shuffle_ps(s1_v1, s1_v1, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 s1_12 = _mm_shuffle_ps(s1_v1, s1_v1, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 o_m10 = _mm_mul_ps(s1_10, s2_v0);
		__m128 o_m11 = _mm_mul_ps(s1_11, s2_v1);
		__m128 o_m12 = _mm_mul_ps(s1_12, s2_v2);
		__m128 o_v1 = _mm_add_ps(_mm_add_ps(o_m10, o_m11), o_m12);
		_mm_storeu_ps(&out.Value[1][0], o_v1);
	}
	{
		__m128 s1_20 = _mm_shuffle_ps(s1_v2, s1_v2, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 s1_21 = _mm_shuffle_ps(s1_v2, s1_v2, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 s1_22 = _mm_shuffle_ps(s1_v2, s1_v2, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 o_m20 = _mm_mul_ps(s1_20, s2_v0);
		__m128 o_m21 = _mm_mul_ps(s1_21, s2_v1);
		__m128 o_m22 = _mm_mul_ps(s1_22, s2_v2);
		__m128 o_v2 = _mm_add_ps(_mm_add_ps(o_m20, o_m21), o_m22);
		_mm_storeu_ps(&out.Value[2][0], o_v2);
		o_v3 = _mm_shuffle_ps(o_v2, o_v2, _MM_SHUFFLE(2, 2, 2, 2));
	}
	{
		EFK_ALIGN_AS(16) const uint32_t mask_u32[4] = {0xffffffff, 0x00000000, 0x00000000, 0x00000000};
		__m128 mask = _mm_load_ps((const float*)mask_u32);
		s2_v0 = _mm_shuffle_ps(s2_v0, s2_v0, _MM_SHUFFLE(2, 1, 0, 0));
		s2_v1 = _mm_shuffle_ps(s2_v1, s2_v1, _MM_SHUFFLE(2, 1, 0, 0));
		s2_v2 = _mm_shuffle_ps(s2_v2, s2_v2, _MM_SHUFFLE(2, 1, 0, 0));
		__m128 s1_30 = _mm_shuffle_ps(s1_v3, s1_v3, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 s1_31 = _mm_shuffle_ps(s1_v3, s1_v3, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 s1_32 = _mm_shuffle_ps(s1_v3, s1_v3, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 o_m30 = _mm_mul_ps(s1_30, s2_v0);
		__m128 o_m31 = _mm_mul_ps(s1_31, s2_v1);
		__m128 o_m32 = _mm_mul_ps(s1_32, s2_v2);
		__m128 o_v3p = _mm_add_ps(_mm_add_ps(o_m30, o_m31), _mm_add_ps(o_m32, s2_v3));
		o_v3 = _mm_or_ps(_mm_and_ps(mask, o_v3), _mm_andnot_ps(mask, o_v3p));
		_mm_storeu_ps(&out.Value[3][0] - 1, o_v3);
	}
#elif defined(EFK_NEON)
	float32x4_t s1_v0 = vld1q_f32(&in1.Value[0][0]);
	float32x4_t s1_v12 = vld1q_f32(&in1.Value[1][1]);
	float32x4_t s1_v3 = vld1q_f32(&in1.Value[2][2]);
	float32x4_t s1_v1 = vextq_f32(s1_v0, s1_v12, 3);
	float32x4_t s1_v2 = vextq_f32(s1_v12, s1_v3, 2);
	float32x4_t s2_v0 = vld1q_f32(&in2.Value[0][0]);
	float32x4_t s2_v12 = vld1q_f32(&in2.Value[1][1]);
	float32x4_t s2_v3 = vld1q_f32(&in2.Value[2][2]);
	float32x4_t s2_v1 = vextq_f32(s2_v0, s2_v12, 3);
	float32x4_t s2_v2 = vextq_f32(s2_v12, s2_v3, 2);
	float o_v3_0;
	{
		float32x4_t o_v0 = vmulq_lane_f32(s2_v0, vget_low_f32(s1_v0), 0);
		float32x4_t o_v1 = vmulq_lane_f32(s2_v0, vget_low_f32(s1_v1), 0);
		float32x4_t o_v2 = vmulq_lane_f32(s2_v0, vget_low_f32(s1_v2), 0);
		o_v0 = vmlaq_lane_f32(o_v0, s2_v1, vget_low_f32(s1_v0), 1);
		o_v1 = vmlaq_lane_f32(o_v1, s2_v1, vget_low_f32(s1_v1), 1);
		o_v2 = vmlaq_lane_f32(o_v2, s2_v1, vget_low_f32(s1_v2), 1);
		o_v0 = vmlaq_lane_f32(o_v0, s2_v2, vget_high_f32(s1_v0), 0);
		o_v1 = vmlaq_lane_f32(o_v1, s2_v2, vget_high_f32(s1_v1), 0);
		o_v2 = vmlaq_lane_f32(o_v2, s2_v2, vget_high_f32(s1_v2), 0);
		vst1q_f32(&out.Value[0][0], o_v0);
		vst1q_f32(&out.Value[1][0], o_v1);
		vst1q_f32(&out.Value[2][0], o_v2);
		o_v3_0 = vgetq_lane_f32(o_v2, 2);
	}
	{
		s2_v0 = vextq_f32(s2_v0, s2_v0, 3);
		s2_v1 = vextq_f32(s2_v1, s2_v1, 3);
		s2_v2 = vextq_f32(s2_v2, s2_v2, 3);
		float32x4_t o_v3 = vmlaq_lane_f32(s2_v3, s2_v0, vget_low_f32(s1_v3), 1);
		o_v3 = vmlaq_lane_f32(o_v3, s2_v1, vget_high_f32(s1_v3), 0);
		o_v3 = vmlaq_lane_f32(o_v3, s2_v2, vget_high_f32(s1_v3), 1);
		vst1q_f32(&out.Value[3][0] - 1, vsetq_lane_f32(o_v3_0, o_v3, 0));
	}
#elif 1
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
//----------------------------------------------------------------------------------


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
//----------------------------------------------------------------------------------
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
void ReadData( T& dst, unsigned char*& pos )
{
	memcpy(&dst, pos, sizeof(T));
	pos += sizeof(T);
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

	float getValue(IRandObject& g) const
	{
		float r;
		r = g.GetRand(min, max);
		return r;
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

	float getValue(IRandObject& g) const
	{
		float r;
		r = g.GetRand((float)min, (float)max);
		return r;
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

	vector2d getValue(IRandObject& g) const
	{
		vector2d r;
		r.x = g.GetRand(min.x, max.x);
		r.y = g.GetRand(min.y, max.y);
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

	void normalize()
	{
		float len = sqrtf(x * x + y * y + z * z);
		if (len > 0.0001f)
		{
			x /= len;
			y /= len;
			z /= len;
		}
		else
		{
			x = 1.0;
			y = 0.0;
			z = 0.0;
		}
	}

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

	vector3d getValue(IRandObject& g) const
	{
		vector3d r;
		r.x = g.GetRand(min.x, max.x);
		r.y = g.GetRand(min.y, max.y);
		r.z = g.GetRand(min.z, max.z);
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

inline Color HSVToRGB(Color hsv) {
	int H = hsv.R, S = hsv.G, V = hsv.B;
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
	Color result;
	result.R = R;
	result.G = G;
	result.B = B;
	result.A = hsv.A;
	return result;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct random_color
{
	ColorMode mode;
	Color	max;
	Color	min;

	void reset()
	{
		assert( sizeof(random_color) == 12 );
		mode = COLOR_MODE_RGBA;
		max = {255, 255, 255, 255};
		min = {255, 255, 255, 255};
	};

	Color getValue(IRandObject& g) const
	{
		Color r = getDirectValue( g );
		if( mode == COLOR_MODE_HSVA )
		{
			r = HSVToRGB( r );
		}
		return r;
	}
	
	Color getDirectValue(IRandObject& g) const
	{
		Color r;
		r.R = (uint8_t) (g.GetRand(min.R, max.R));
		r.G = (uint8_t) (g.GetRand(min.G, max.G));
		r.B = (uint8_t) (g.GetRand(min.B, max.B));
		r.A = (uint8_t) (g.GetRand(min.A, max.A));
		return r;
	}

	void load( int version, unsigned char*& pos )
	{
		if( version >= 4 )
		{
			uint8_t mode_ = 0;
			ReadData<uint8_t>(mode_, pos);
			mode = static_cast<ColorMode>(mode_);
			pos++;	// reserved
		}
		else
		{
			mode = COLOR_MODE_RGBA;
		}
		ReadData<Color>(max, pos );
		ReadData<Color>(min, pos );
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

	void setValueToArg( Color& o, const Color& start_, const Color& end_, float t ) const
	{
		assert( start.mode == end.mode );
		float d = easingA * t * t * t + easingB * t * t + easingC * t;
		o = Color::Lerp(start_, end_, d);
		if( start.mode == COLOR_MODE_HSVA )
		{
			o = HSVToRGB( o );
		}
	}

	Color getStartValue(IRandObject& g) const
	{
		return start.getDirectValue( g );
	}
	
	Color getEndValue(IRandObject& g) const
	{
		return end.getDirectValue( g);
	}

	void load( int version, unsigned char*& pos )
	{
		start.load( version, pos );
		end.load( version, pos );
		ReadData<float>(easingA, pos );
		ReadData<float>(easingB, pos );
		ReadData<float>(easingC, pos );
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INTERNAL_STRUCT_H__
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

	size = (int32_t)reader->GetLength();
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
//----------------------------------------------------------------------------------
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

	float GetOffset( InstanceGlobal& g ) const;

	void SetDefaultValue( float value ) { m_defaultValue = value; }

	void ChangeCoordinate();

	void Maginify(float value );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class FCurveVector2D
{
public:
	FCurve X;
	FCurve Y;
	
	FCurveVector2D();
	int32_t Load(void* data, int32_t version);
};

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
enum class BindType : int32_t
{
	NotBind = 0,
	NotBind_Root = 3,
	WhenCreating = 1,
	Always = 2,
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
			Color all;
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
		if (type == FCurve_RGBA)
		{
			ES_SAFE_DELETE(fcurve_rgba.FCurve);
		}
	}

	void load(uint8_t*& pos, int32_t version)
	{
		memcpy(&type, pos, sizeof(int));
		pos += sizeof(int);

		if (type == Fixed)
		{
			memcpy(&fixed, pos, sizeof(fixed));
			pos += sizeof(fixed);
		}
		else if (type == Random)
		{
			random.all.load(version, pos);
		}
		else if (type == Easing)
		{
			easing.all.load(version, pos);
		}
		else if (type == FCurve_RGBA)
		{
			fcurve_rgba.FCurve = new FCurveVectorColor();
			int32_t size = fcurve_rgba.FCurve->Load(pos, version);
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
	int MaxGeneration = 1;
	BindType TranslationBindType = BindType::Always;
	BindType RotationBindType = BindType::Always;
	BindType ScalingBindType = BindType::Always;
	int RemoveWhenLifeIsExtinct = 1;
	int RemoveWhenParentIsRemoved = 0;
	int RemoveWhenChildrenIsExtinct = 0;
	random_int life;
	random_float GenerationTime;
	random_float GenerationTimeOffset;

	ParameterCommonValues()
	{
		life.max = 1;
		life.min = 1;
		GenerationTime.max = 1;
		GenerationTime.min = 1;
		GenerationTimeOffset.max = 0;
		GenerationTimeOffset.min = 0;
	}
};

struct ParameterDepthValues
{
	float	DepthOffset;
	bool	IsDepthOffsetScaledWithCamera;
	bool	IsDepthOffsetScaledWithParticleScale;
	ZSortType	ZSort;
	int32_t	DrawingPriority;
	float	SoftParticle;

	ParameterDepthValues()
	{
		DepthOffset = 0;
		IsDepthOffsetScaledWithCamera = false;
		IsDepthOffsetScaledWithParticleScale = false;
		ZSort = ZSortType::None;
		DrawingPriority = 0;
		SoftParticle = 0.0f;
	}
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
enum class LocationAbsType : int32_t
{
	None = 0,
	Gravity = 1,
	AttractiveForce = 2,
};

struct LocationAbsParameter
{
	LocationAbsType type = LocationAbsType::None;

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

	enum class AxisType : int32_t
	{
		X,
		Y,
		Z,
	};

	enum
	{
		TYPE_POINT = 0,
		TYPE_SPHERE = 1,
		TYPE_MODEL = 2,
		TYPE_CIRCLE = 3,
		TYPE_LINE = 4,

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

	enum class LineType : int32_t
	{
		Random = 0,
		Order = 1,
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
			AxisType		axisDirection;
			random_float	angle_noize;
		} circle;

		struct
		{
			int32_t			division;
			random_vector3d	position_start;
			random_vector3d	position_end;
			random_float	position_noize;
			LineType		type;
		} line;
	};

	void load(uint8_t*& pos, int32_t version)
	{
		memcpy(&EffectsRotation, pos, sizeof(int));
		pos += sizeof(int);

		memcpy(&type, pos, sizeof(int));
		pos += sizeof(int);

		if (type == TYPE_POINT)
		{
			memcpy(&point, pos, sizeof(point));
			pos += sizeof(point);
		}
		else if (type == TYPE_SPHERE)
		{
			memcpy(&sphere, pos, sizeof(sphere));
			pos += sizeof(sphere);
		}
		else if (type == TYPE_MODEL)
		{
			memcpy(&model, pos, sizeof(model));
			pos += sizeof(model);
		}
		else if (type == TYPE_CIRCLE)
		{
			if (version < 10)
			{
				memcpy(&circle, pos, sizeof(circle) - sizeof(circle.axisDirection) - sizeof(circle.angle_noize));
				pos += sizeof(circle) - sizeof(circle.axisDirection) - sizeof(circle.angle_noize);
				circle.axisDirection = AxisType::Z;
				circle.angle_noize.max = 0;
				circle.angle_noize.min = 0;
			}
			else
			{
				memcpy(&circle, pos, sizeof(circle));
				pos += sizeof(circle);
			}
		}
		else if (type == TYPE_LINE)
		{
			memcpy(&line, pos, sizeof(line));
			pos += sizeof(line);
		}
	}
};

struct ParameterRendererCommon
{
	int32_t				ColorTextureIndex;
	AlphaBlendType		AlphaBlend;

	TextureFilterType	FilterType;

	TextureWrapType		WrapType;

	bool				ZWrite;

	bool				ZTest;

	bool				Distortion;

	float				DistortionIntensity;

	BindType			ColorBindType;

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
		UV_FCURVE = 4,

		UV_DWORD = 0x7fffffff,
	} UVType;


	/**
	@brief	UV Parameter
	@note
	for Compatibility
	*/
	struct UVScroll_09
	{
		rectf		Position;
		vector2d	Speed;
	};

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

			random_int	StartFrame;

		} Animation;

		struct
		{
			random_vector2d	Position;
			random_vector2d	Size;
			random_vector2d	Speed;
		} Scroll;

		struct
		{
			FCurveVector2D* Position;
			FCurveVector2D* Size;
		} FCurve;

	} UV;

	void reset()
	{
		memset(this, 0, sizeof(ParameterRendererCommon));
	}

	void load(uint8_t*& pos, int32_t version)
	{
		memset(this, 0, sizeof(ParameterRendererCommon));

		memcpy(&ColorTextureIndex, pos, sizeof(int));
		pos += sizeof(int);

		memcpy(&AlphaBlend, pos, sizeof(int));
		pos += sizeof(int);

		memcpy(&FilterType, pos, sizeof(int));
		pos += sizeof(int);

		memcpy(&WrapType, pos, sizeof(int));
		pos += sizeof(int);

		if (version >= 5)
		{
			int32_t zwrite, ztest = 0;

			memcpy(&ztest, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			memcpy(&zwrite, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			ZWrite = zwrite != 0;
			ZTest = ztest != 0;
		}
		else
		{
			ZWrite = false;
			ZTest = true;
		}

		memcpy(&FadeInType, pos, sizeof(int));
		pos += sizeof(int);

		if (FadeInType == FADEIN_ON)
		{
			memcpy(&FadeIn, pos, sizeof(FadeIn));
			pos += sizeof(FadeIn);
		}

		memcpy(&FadeOutType, pos, sizeof(int));
		pos += sizeof(int);

		if (FadeOutType == FADEOUT_ON)
		{
			memcpy(&FadeOut, pos, sizeof(FadeOut));
			pos += sizeof(FadeOut);
		}

		memcpy(&UVType, pos, sizeof(int));
		pos += sizeof(int);

		if (UVType == UV_DEFAULT)
		{
		}
		else if (UVType == UV_FIXED)
		{
			memcpy(&UV.Fixed, pos, sizeof(UV.Fixed));
			pos += sizeof(UV.Fixed);
		}
		else if (UVType == UV_ANIMATION)
		{
			if (version < 10)
			{
				// without start frame
				memcpy(&UV.Animation, pos, sizeof(UV.Animation) - sizeof(UV.Animation.StartFrame));
				pos += sizeof(UV.Animation) - sizeof(UV.Animation.StartFrame);
				UV.Animation.StartFrame.max = 0;
				UV.Animation.StartFrame.min = 0;
			}
			else
			{
				memcpy(&UV.Animation, pos, sizeof(UV.Animation));
				pos += sizeof(UV.Animation);
			}
		}
		else if (UVType == UV_SCROLL)
		{
			if (version < 10)
			{
				// compatibility
				UVScroll_09 values;
				memcpy(&values, pos, sizeof(values));
				pos += sizeof(values);
				UV.Scroll.Position.max.x = values.Position.x;
				UV.Scroll.Position.max.y = values.Position.y;
				UV.Scroll.Position.min = UV.Scroll.Position.max;

				UV.Scroll.Size.max.x = values.Position.w;
				UV.Scroll.Size.max.y = values.Position.h;
				UV.Scroll.Size.min = UV.Scroll.Size.max;

				UV.Scroll.Speed.max.x = values.Speed.x;
				UV.Scroll.Speed.max.y = values.Speed.y;
				UV.Scroll.Speed.min = UV.Scroll.Speed.max;

			}
			else
			{
				memcpy(&UV.Scroll, pos, sizeof(UV.Scroll));
				pos += sizeof(UV.Scroll);
			}
		}
		else if (UVType == UV_FCURVE)
		{
			UV.FCurve.Position = new FCurveVector2D();
			UV.FCurve.Size = new FCurveVector2D();
			pos += UV.FCurve.Position->Load(pos, version);
			pos += UV.FCurve.Size->Load(pos, version);
		}

		if (version >= 10)
		{
			memcpy(&ColorBindType, pos, sizeof(int32_t));
			pos += sizeof(int32_t);
		}
		else
		{
			ColorBindType = BindType::NotBind;
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

	void destroy()
	{
		if (UVType == UV_FCURVE)
		{
			ES_SAFE_DELETE(UV.FCurve.Position);
			ES_SAFE_DELETE(UV.FCurve.Size);
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
	void LoadParameter(unsigned char*& pos, EffectNode* parent, Setting* setting);

	// 初期化
	void Initialize();

public:

	/**
	@brief	\~english Whether to draw the node.
	\~japanese このノードを描画するか?

	@note
	\~english 普通は描画されないノードは、描画の種類が変更されて、描画しないノードになる。ただし、色の継承をする場合、描画のみを行わないノードになる。
	\~japanese For nodes that are not normally rendered, the rendering type is changed to become a node that does not render. However, when color inheritance is done, it becomes a node which does not perform drawing only.
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

	ParameterDepthValues		DepthValues;

	ParameterRendererCommon		RendererCommon;

	ParameterSoundType			SoundType;
	ParameterSound				Sound;

	eRenderingOrder				RenderingOrder;

	int32_t						RenderingPriority = -1;

	Effect* GetEffect() const override;

	int GetChildrenCount() const override;

	EffectNode* GetChild(int index) const override;

	EffectBasicRenderParameter GetBasicRenderParameter() override;

	void SetBasicRenderParameter(EffectBasicRenderParameter param) override;

	EffectModelParameter GetEffectModelParameter() override;

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

	virtual void EndRenderingGroup(InstanceGroup* group, Manager* manager);

	/**
	@brief	描画
	*/
	virtual void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager);

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
	virtual void InitializeRenderedInstance(Instance& instance, Manager* manager);

	/**
	@brief	描画部分更新
	*/
	virtual void UpdateRenderedInstance(Instance& instance, Manager* manager);

	/**
	@brief	描画部分更新
	*/
	virtual float GetFadeAlpha(const Instance& instance);

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
		Color _color;
		Color _original;

		union 
		{
			struct
			{
				Color _color;
			} fixed;

			struct
			{
				Color _color;
			} random;

			struct
			{
				Color start;
				Color  end;

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
	BillboardType	Billboard;
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

	void LoadRendererParameter(unsigned char*& pos, Setting* setting) override;

	void BeginRendering(int32_t count, Manager* manager) override;

	void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager) override;

	void EndRendering(Manager* manager) override;

	void InitializeRenderedInstance(Instance& instance, Manager* manager) override;

	void UpdateRenderedInstance(Instance& instance, Manager* manager) override;

	eEffectNodeType GetType() const override { return EFFECT_NODE_TYPE_MODEL; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_MODEL_H__

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
			Color all;
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
			Color l;
			Color r;
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
		Color _color;
		Color _original;

		union
		{
			struct
			{
				Color _color;
			} fixed;

			struct
			{
				Color _color;
			} random;

			struct
			{
				Color start;
				Color  end;

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

	int32_t	SplineDivision = 1;

	EffectNodeRibbon(Effect* effect, unsigned char*& pos)
		: EffectNodeImplemented(effect, pos)
	{
	}

	~EffectNodeRibbon()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting) override;

	void BeginRendering(int32_t count, Manager* manager) override;

	void BeginRenderingGroup(InstanceGroup* group, Manager* manager) override;

	void EndRenderingGroup(InstanceGroup* group, Manager* manager) override;

	void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager) override;

	void EndRendering(Manager* manager) override;

	void InitializeRenderedInstance(Instance& instance, Manager* manager) override;

	void UpdateRenderedInstance(Instance& instance, Manager* manager) override;

	eEffectNodeType GetType() const override { return EFFECT_NODE_TYPE_RIBBON; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_RIBBON_H__

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
		Color fixed;
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
	Color	current;
	Color	original;

	union
	{
		struct
		{
			Color _color;
		} fixed;

		struct
		{
			Color _color;
		} random;

		struct
		{
			Color  start;
			Color  end;
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

	void LoadRendererParameter(unsigned char*& pos, Setting* setting) override;

	void BeginRendering(int32_t count, Manager* manager) override;

	void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager) override;

	void EndRendering(Manager* manager) override;

	void InitializeRenderedInstance(Instance& instance, Manager* manager) override;

	void UpdateRenderedInstance(Instance& instance, Manager* manager) override;

	eEffectNodeType GetType() const override { return EFFECT_NODE_TYPE_RING; }

private:
	void LoadSingleParameter( unsigned char*& pos, RingSingleParameter& param );

	void LoadLocationParameter( unsigned char*& pos, RingLocationParameter& param );
	
	void LoadColorParameter( unsigned char*& pos, RingColorParameter& param );
	
	void InitializeSingleValues(const RingSingleParameter& param, RingSingleValues& values, Manager* manager, InstanceGlobal* instanceGlobal);

	void InitializeLocationValues(const RingLocationParameter& param, RingLocationValues& values, Manager* manager, InstanceGlobal* instanceGlobal);
	
	void InitializeColorValues(const RingColorParameter& param, RingColorValues& values, Manager* manager, InstanceGlobal* instanceGlobal);
	
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
			Color ll;
			Color lr;
			Color ul;
			Color ur;
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
		Color _color;

		Color _originalColor;
		
		union 
		{
			struct
			{
				Color _color;
			} fixed;

			struct
			{
				Color _color;
			} random;

			struct
			{
				Color start;
				Color  end;

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

	void LoadRendererParameter(unsigned char*& pos, Setting* setting) override;

	void BeginRendering(int32_t count, Manager* manager) override;

	void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager) override;

	void EndRendering(Manager* manager) override;

	void InitializeRenderedInstance(Instance& instance, Manager* manager) override;

	void UpdateRenderedInstance(Instance& instance, Manager* manager) override;

	eEffectNodeType GetType() const override { return EFFECT_NODE_TYPE_SPRITE; }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_SPRITE_H__

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
					Effekseer::Color color_;
				} fixed;

				struct
				{
					Effekseer::Color color_;
				} random;

				struct
				{
					Effekseer::Color start;
					Effekseer::Color  end;
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
		Color	colorLeft;
		Color	colorCenter;
		Color	colorRight;

		Color	colorLeftMiddle;
		Color	colorCenterMiddle;
		Color	colorRightMiddle;

		Color	_colorLeft;
		Color	_colorCenter;
		Color	_colorRight;

		Color	_colorLeftMiddle;
		Color	_colorCenterMiddle;
		Color	_colorRightMiddle;

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

	int32_t	SplineDivision = 1;

	EffectNodeTrack(Effect* effect, unsigned char*& pos)
		: EffectNodeImplemented(effect, pos)
		, TrackTexture(-1)
	{
	}

	~EffectNodeTrack()
	{
	}

	void LoadRendererParameter(unsigned char*& pos, Setting* setting) override;

	void BeginRendering(int32_t count, Manager* manager) override;

	void BeginRenderingGroup(InstanceGroup* group, Manager* manager) override;

	void EndRenderingGroup(InstanceGroup* group, Manager* manager) override;

	void Rendering(const Instance& instance, const Instance* next_instance, Manager* manager) override;

	void EndRendering(Manager* manager) override;

	void InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager) override;

	void InitializeRenderedInstance(Instance& instance, Manager* manager) override;

	void UpdateRenderedInstance(Instance& instance, Manager* manager) override;

	eEffectNodeType GetType() const override { return EFFECT_NODE_TYPE_TRACK; }

	void InitializeValues(InstanceGroupValues::Color& value, StandardColorParameter& param, InstanceGlobal* instanceGlobal);
	void InitializeValues(InstanceGroupValues::Size& value, TrackSizeParameter& param, Manager* manager);
	void SetValues(Color& c, const Instance& instance, InstanceGroupValues::Color& value, StandardColorParameter& param, int32_t time, int32_t livedTime);
	void SetValues(float& s, InstanceGroupValues::Size& value, TrackSizeParameter& param, float time);
	void LoadValues(TrackSizeParameter& param, unsigned char*& pos);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_ParameterNODE_TRACK_H__

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

/**
	@brief	A class to backup resorces when effect is reloaded
*/
class EffectReloadingBackup
{
public:
	template<class T> 
	class Holder
	{
	public:
		T value;
		int counter = 0;
	};

	template<class T>
	class HolderCollection
	{
		std::map<std::u16string, Holder<T>> collection;

	public:
		void Push(const char16_t* key, T value)
		{
			auto key_ = std::u16string(key);
			auto it = collection.find(key_);
			
			if (it == collection.end())
			{
				collection[key_].value = value;
				collection[key_].counter = 1;
			}
			else
			{
				assert(it->second.value == value);
				it->second.counter++;
			}
		}

		bool Pop(const char16_t* key, T& value)
		{
			auto key_ = std::u16string(key);
			auto it = collection.find(key_);

			if (it == collection.end())
			{
				return false;
			}
			else
			{
				it->second.counter--;
				value = it->second.value;
				if (it->second.counter == 0)
				{
					collection.erase(it);
				}
				return true;
			}
		}

		std::map<std::u16string, Holder<T>>& GetCollection()
		{
			return collection;
		}
	};

	HolderCollection<TextureData*> images;
	HolderCollection<TextureData*> normalImages;
	HolderCollection<TextureData*> distortionImages;
	HolderCollection<void*> sounds;
	HolderCollection<void*> models;
};

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
	friend class EffectNodeImplemented;
private:
	ManagerImplemented* m_pManager;

	Setting*	m_setting;

	mutable std::atomic<int32_t> m_reference;

	int	m_version;

	int	m_ImageCount;
	EFK_CHAR**		m_ImagePaths;
	TextureData**	m_pImages;

	int	m_normalImageCount;
	EFK_CHAR**		m_normalImagePaths;
	TextureData**	m_normalImages;
	
	int	m_distortionImageCount;
	EFK_CHAR**		m_distortionImagePaths;
	TextureData**	m_distortionImages;

	int	m_WaveCount;
	EFK_CHAR**		m_WavePaths;
	void**			m_pWaves;

	int32_t	m_modelCount;
	EFK_CHAR**		m_modelPaths;
	void**			m_pModels;

	std::u16string name_;
	std::basic_string<EFK_CHAR>		m_materialPath;

	int32_t			renderingNodesCount = 0;
	int32_t			renderingNodesThreshold = 0;

	/* 拡大率 */
	float	m_maginification;

	float	m_maginificationExternal;

	// default random seed
	int32_t	m_defaultRandomSeed;

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

	//! a flag to reload
	bool isReloadingOnRenderingThread = false;

	//! backup to reload on rendering thread
	std::unique_ptr<EffectReloadingBackup> reloadingBackup;

	void ResetReloadingBackup();

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
	EffectNode* GetRoot() const override;

	float GetMaginification() const override;

	bool Load( void* pData, int size, float mag, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType);

	/**
		@breif	何も読み込まれていない状態に戻す
	*/
	void Reset();

	/**
		@brief	Compatibility for magnification.
	*/
	bool IsDyanamicMagnificationValid() const;

private:
	/**
		@brief	マネージャー取得
	*/
	Manager* GetManager() const;

public:
	const char16_t* GetName() const override;

	void SetName(const char16_t* name) override;

	/**
	@brief	設定取得
	*/
	Setting* GetSetting() const override;
	
	/**
		@brief	エフェクトデータのバージョン取得
	*/
	int GetVersion() const override;

	/**
		@brief	格納されている画像のポインタを取得する。
	*/
	TextureData* GetColorImage(int n) const override;

	/**
		@brief	格納されている画像のポインタの個数を取得する。
	*/
	int32_t GetColorImageCount() const override;

	/**
	@brief	格納されている画像のポインタを取得する。
	*/
	TextureData* GetNormalImage(int n) const override;

	int32_t GetNormalImageCount() const override;

	TextureData* GetDistortionImage(int n) const override;

	int32_t GetDistortionImageCount() const override;

	/**
		@brief	格納されている音波形のポインタを取得する。
	*/
	void* GetWave( int n ) const override;

	int32_t GetWaveCount() const override;

	/**
		@brief	格納されているモデルのポインタを取得する。
	*/
	void* GetModel( int n ) const override;

	int32_t GetModelCount() const override;

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( void* data, int32_t size, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType) override;

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( const EFK_CHAR* path, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType) override;

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( Manager** managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType) override;

	/**
		@brief	エフェクトのリロードを行う。
	*/
	bool Reload( Manager** managers, int32_t managersCount, const EFK_CHAR* path, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType) override;

	/**
		@brief	画像等リソースの再読み込みを行う。
	*/
	void ReloadResources( const EFK_CHAR* materialPath ) override;

	void UnloadResources(const EFK_CHAR* materialPath);

	void UnloadResources() override;

	virtual int GetRef() override { return ReferenceObject::GetRef(); }
	virtual int AddRef() override { return ReferenceObject::AddRef(); }
	virtual int Release() override { return ReferenceObject::Release(); }
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_EFFECT_IMPLEMENTED_H__

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

		bool				IsPreupdated = false;
		int32_t				StartFrame = 0;
		int32_t Layer = 0;

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
			, DoUseBaseMatrix			( false )
			, RemovingCallback			(NULL)
			, Speed						( 1.0f )
			, Self						( -1 )
		{
		
		}

		Matrix43* GetEnabledGlobalMatrix();

		void CopyMatrixFromInstanceToRoot();
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

	// mutex for rendering
	std::mutex					m_renderingMutex;
	bool						m_isLockedWithRenderingMutex = false;

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
	InstanceContainer* CreateInstanceContainer( EffectNode* pEffectNode, InstanceGlobal* pGlobal, bool isRoot, const Matrix43& rootMatrix, Instance* pParent);

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
	void Destroy() override;

	/**
		@brief	更新番号を取得する。
	*/
	uint32_t GetSequenceNumber() const;

	/**
		@brief	メモリ確保関数取得
	*/
	MallocFunc GetMallocFunc() const override;

	/**
		@brief	メモリ確保関数設定
	*/
	void SetMallocFunc( MallocFunc func ) override;

	/**
		@brief	メモリ破棄関数取得
	*/
	FreeFunc GetFreeFunc() const override;

	/**
		@brief	メモリ破棄関数設定
	*/
	void SetFreeFunc( FreeFunc func ) override;

	/**
		@brief	ランダム関数取得
	*/
	RandFunc GetRandFunc() const override;

	/**
		@brief	ランダム関数設定
	*/
	void SetRandFunc( RandFunc func ) override;

	/**
		@brief	ランダム最大値取得
	*/
	int GetRandMax() const override;

	/**
		@brief	ランダム関数設定
	*/
	void SetRandMax( int max_ ) override;

	/**
		@brief	座標系を取得する。
	*/
	CoordinateSystem GetCoordinateSystem() const override;

	/**
		@brief	座標系を設定する。
	*/
	void SetCoordinateSystem( CoordinateSystem coordinateSystem ) override;

	/**
		@brief	スプライト描画機能取得
	*/
	SpriteRenderer* GetSpriteRenderer() override;

	/**
		@brief	スプライト描画機能設定
	*/
	void SetSpriteRenderer( SpriteRenderer* renderer ) override;

	/**
		@brief	リボン描画機能取得
	*/
	RibbonRenderer* GetRibbonRenderer() override;

	/**
		@brief	リボン描画機能設定
	*/
	void SetRibbonRenderer( RibbonRenderer* renderer ) override;

	/**
		@brief	リング描画機能取得
	*/
	RingRenderer* GetRingRenderer() override;

	/**
		@brief	リング描画機能設定
	*/
	void SetRingRenderer( RingRenderer* renderer ) override;

	/**
		@brief	モデル描画機能取得
	*/
	ModelRenderer* GetModelRenderer() override;

	/**
		@brief	モデル描画機能設定
	*/
	void SetModelRenderer( ModelRenderer* renderer ) override;

	/**
		@brief	軌跡描画機能取得
	*/
	TrackRenderer* GetTrackRenderer() override;

	/**
		@brief	軌跡描画機能設定
	*/
	void SetTrackRenderer( TrackRenderer* renderer ) override;

	/**
		@brief	設定クラスを取得する。
	*/
	Setting* GetSetting() override;

	/**
		@brief	設定クラスを設定する。
		@param	setting	[in]	設定
	*/
	void SetSetting(Setting* setting) override;

	/**
		@brief	エフェクト読込クラスを取得する。
	*/
	EffectLoader* GetEffectLoader() override;

	/**
		@brief	エフェクト読込クラスを設定する。
	*/
	void SetEffectLoader( EffectLoader* effectLoader ) override;

	/**
		@brief	テクスチャ読込クラスを取得する。
	*/
	TextureLoader* GetTextureLoader() override;

	/**
		@brief	テクスチャ読込クラスを設定する。
	*/
	void SetTextureLoader( TextureLoader* textureLoader ) override;
	
	/**
		@brief	サウンド再生取得
	*/
	SoundPlayer* GetSoundPlayer() override;

	/**
		@brief	サウンド再生機能設定
	*/
	void SetSoundPlayer( SoundPlayer* soundPlayer ) override;
	
	/**
		@brief	サウンド再生取得
	*/
	SoundLoader* GetSoundLoader() override;

	/**
		@brief	サウンド再生機能設定
	*/
	void SetSoundLoader( SoundLoader* soundLoader ) override;

	/**
		@brief	モデル読込クラスを取得する。
	*/
	ModelLoader* GetModelLoader() override;

	/**
		@brief	モデル読込クラスを設定する。
	*/
	void SetModelLoader( ModelLoader* modelLoader ) override;
	
	/**
		@brief	エフェクト停止
	*/
	void StopEffect( Handle handle ) override;

	/**
		@brief	全てのエフェクト停止
	*/
	void StopAllEffects() override;

	/**
		@brief	エフェクト停止
	*/
	void StopRoot( Handle handle ) override;

	/**
		@brief	エフェクト停止
	*/
	void StopRoot( Effect* effect ) override;

	/**
		@brief	エフェクトのインスタンスが存在しているか取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	存在してるか?
	*/
	bool Exists( Handle handle ) override;

	int32_t GetInstanceCount( Handle handle ) override;

	/**
		@brief	エフェクトのインスタンスに設定されている行列を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	行列
	*/
	Matrix43 GetMatrix( Handle handle ) override;

	/**
		@brief	エフェクトのインスタンスに変換行列を設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	mat		[in]	変換行列
	*/
	void SetMatrix( Handle handle, const Matrix43& mat ) override;

	Vector3D GetLocation( Handle handle ) override;
	void SetLocation( Handle handle, float x, float y, float z ) override;
	void SetLocation( Handle handle, const Vector3D& location ) override;
	void AddLocation( Handle handle, const Vector3D& location ) override;

	/**
		@brief	エフェクトのインスタンスの回転角度を指定する。(ラジアン)
	*/
	void SetRotation( Handle handle, float x, float y, float z ) override;

	/**
		@brief	エフェクトのインスタンスの任意軸周りの反時計周りの回転角度を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	axis	[in]	軸
		@param	angle	[in]	角度(ラジアン)
	*/
	void SetRotation( Handle handle, const Vector3D& axis, float angle ) override;

	/**
		@brief	エフェクトのインスタンスの拡大率を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	x		[in]	X方向拡大率
		@param	y		[in]	Y方向拡大率
		@param	z		[in]	Z方向拡大率
	*/
	void SetScale( Handle handle, float x, float y, float z ) override;

	void SetAllColor(Handle handle, Color color) override;

	// エフェクトのターゲット位置を指定する。
	void SetTargetLocation( Handle handle, float x, float y, float z ) override;
	void SetTargetLocation( Handle handle, const Vector3D& location ) override;

	Matrix43 GetBaseMatrix( Handle handle ) override;

	void SetBaseMatrix( Handle handle, const Matrix43& mat ) override;

	/**
		@brief	エフェクトのインスタンスに廃棄時のコールバックを設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	callback	[in]	コールバック
	*/
	void SetRemovingCallback( Handle handle, EffectInstanceRemovingCallback callback ) override;

	bool GetShown(Handle handle) override;

	void SetShown( Handle handle, bool shown ) override;
	
	bool GetPaused(Handle handle) override;

	void SetPaused( Handle handle, bool paused ) override;

	void SetPausedToAllEffects(bool paused) override;

	int GetLayer(Handle handle) override;

	void SetLayer(Handle handle, int32_t layer)  override;

	float GetSpeed(Handle handle) const override;

	void SetSpeed( Handle handle, float speed ) override;
	
	void SetAutoDrawing( Handle handle, bool autoDraw ) override;
	
	void Flip() override;

	/**
		@brief	更新処理
	*/
	void Update( float deltaFrame ) override;

	/**
		@brief	更新処理を開始する。
		@note
		Updateを実行する際は、実行する必要はない。
	*/
	void BeginUpdate() override;

	/**
		@brief	更新処理を終了する。
		@note
		Updateを実行する際は、実行する必要はない。
	*/
	void EndUpdate() override;

	/**
		@brief	ハンドル単位の更新を行う。
		@param	handle		[in]	ハンドル
		@param	deltaFrame	[in]	更新するフレーム数(60fps基準)
		@note
		更新する前にBeginUpdate、更新し終わった後にEndUpdateを実行する必要がある。
	*/
	void UpdateHandle( Handle handle, float deltaFrame = 1.0f ) override;

private:
	void UpdateHandle( DrawSet& drawSet, float deltaFrame );

	void Preupdate(DrawSet& drawSet);
public:

	/**
		@brief	描画処理
	*/
	void Draw(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) override;
	
	void DrawBack(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) override;

	void DrawFront(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) override;

	void DrawHandle( Handle handle ) override;

	void DrawHandleBack(Handle handle) override;

	void DrawHandleFront(Handle handle) override;

	Handle Play( Effect* effect, float x, float y, float z ) override;

	Handle Play(Effect* effect, const Vector3D& position, int32_t startFrame) override;
	
	int GetCameraCullingMaskToShowAllEffects() override;

	/**
		@brief	Update処理時間を取得。
	*/
	int GetUpdateTime() const override {return m_updateTime;};
	
	/**
		@brief	Draw処理時間を取得。
	*/
	int GetDrawTime() const override {return m_drawTime;};

	/**
		@brief	残りの確保したインスタンス数を取得する。
	*/
	virtual int32_t GetRestInstancesCount() const override { return (int32_t)m_reserved_instances.size(); }

	/**
		@brief	start reload
	*/
	void BeginReloadEffect( Effect* effect, bool doLockThread );

	/**
		@brief	end reload
	*/
	void EndReloadEffect( Effect* effect, bool doLockThread);

	/**
		@brief	エフェクトをカリングし描画負荷を減らすための空間を生成する。
		@param	xsize	X方向幅
		@param	ysize	Y方向幅
		@param	zsize	Z方向幅
		@param	layerCount	層数(大きいほどカリングの効率は上がるがメモリも大量に使用する)
	*/
	void CreateCullingWorld( float xsize, float ysize, float zsize, int32_t layerCount) override;

	/**
		@brief	カリングを行い、カリングされたオブジェクトのみを描画するようにする。
		@param	cameraProjMat	カメラプロジェクション行列
		@param	isOpenGL		OpenGLによる描画か?
	*/
	void CalcCulling(const Matrix44& cameraProjMat, bool isOpenGL) override;

	/**
		@brief	現在存在するエフェクトのハンドルからカリングの空間を配置しなおす。
	*/
	void RessignCulling() override;

	virtual int GetRef() override { return ReferenceObject::GetRef(); }
	virtual int AddRef() override { return ReferenceObject::AddRef(); }
	virtual int Release() override { return ReferenceObject::Release(); }
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_MANAGER_IMPLEMENTED_H__

#ifndef	__EFFEKSEER_INTRUSIVE_LIST_H__
#define	__EFFEKSEER_INTRUSIVE_LIST_H__

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
	@brief	Intrusive List
	@code
	class Instance : public IntrusiveList<Instance> {...};
	@endcode
*/
template <typename T>
class IntrusiveList final
{
public:
	typedef T Type;
	
	class Iterator
	{
		Type* m_Node = nullptr;
	public:
		Iterator() = default;
		Iterator( const Iterator& it ) = default;
		Iterator( Type* node ): m_Node(node) {}
		Type* operator*() const { assert( m_Node != nullptr ); return m_Node; }
		Type* operator->() const { assert( m_Node != nullptr ); return m_Node; }
		Iterator& operator++() { assert( m_Node != nullptr ); m_Node = m_Node->m_NextNode; return *this; }
		Iterator operator++(int) { assert( m_Node != nullptr ); Iterator it(m_Node); m_Node = m_Node->m_NextNode; return it; }
		bool operator==( const Iterator& rhs ) const { return m_Node == rhs.m_Node; }
		bool operator!=( const Iterator& rhs ) const { return m_Node != rhs.m_Node; }
	};
	
	class ReverseIterator
	{
		Type* m_Node = nullptr;
	public:
		ReverseIterator() = default;
		ReverseIterator( const ReverseIterator& it ) = default;
		ReverseIterator( Type* node ): m_Node(node) {}
		Type* operator*() const { assert( m_Node != nullptr ); return m_Node; }
		Type* operator->() const { assert( m_Node != nullptr ); return m_Node; }
		ReverseIterator& operator++() { assert( m_Node != nullptr ); m_Node = m_Node->m_PrevNode; return *this; }
		ReverseIterator operator++(int) { assert( m_Node != nullptr ); ReverseIterator it(m_Node); m_Node = m_Node->m_NextNode; return it; }
		bool operator==( const ReverseIterator& rhs ) const { return m_Node == rhs.m_Node; }
		bool operator!=( const ReverseIterator& rhs ) const { return m_Node != rhs.m_Node; }
	};
	
	class Node
	{
		friend class IntrusiveList<Type>;
		friend class IntrusiveList<Type>::Iterator;
		
	private:
		Type* m_PrevNode = nullptr;
		Type* m_NextNode = nullptr;
	};

private:
	Type* m_HeadNode = nullptr;
	Type* m_TailNode = nullptr;
	size_t m_Count = 0;

public:
	IntrusiveList() = default;
	IntrusiveList( const IntrusiveList<T>& rhs ) = delete;
	IntrusiveList<T>& operator=( const IntrusiveList<T>& rhs ) = delete;
	IntrusiveList( IntrusiveList<T>&& rhs );
	IntrusiveList<T>& operator=( IntrusiveList<T>&& rhs );
	~IntrusiveList();

	void push_back( Type* newObject );
	void pop_back();
	void push_front( Type* newObject );
	void pop_front();
		
	Iterator insert( Iterator it, Type* newObject );
	Iterator erase( Iterator it );
	void clear();
		
	Type* front() const;
	Type* back() const;

	bool empty() const { return m_Count == 0; }
	size_t size() const { return m_Count; }

	Iterator begin() const { return Iterator( m_HeadNode ); }
	Iterator end() const { return Iterator( nullptr ); }
	ReverseIterator rbegin() const { return ReverseIterator( m_TailNode ); }
	ReverseIterator rend() const { return ReverseIterator( nullptr ); }
};

template <typename T>
IntrusiveList<T>::IntrusiveList( IntrusiveList<T>&& rhs )
{
	m_HeadNode = rhs.m_HeadNode;
	m_TailNode = rhs.m_TailNode;
	m_Count = rhs.m_Count;
	rhs.m_HeadNode = nullptr;
	rhs.m_TailNode = nullptr;
	rhs.m_Count = 0;
}

template <typename T>
IntrusiveList<T>& IntrusiveList<T>::operator=( IntrusiveList<T>&& rhs )
{
	m_HeadNode = rhs.m_HeadNode;
	m_TailNode = rhs.m_TailNode;
	m_Count = rhs.m_Count;
	rhs.m_HeadNode = nullptr;
	rhs.m_TailNode = nullptr;
	rhs.m_Count = 0;
}

template <typename T>
IntrusiveList<T>::~IntrusiveList()
{
	clear();
}

template <typename T>
inline void IntrusiveList<T>::push_back( typename IntrusiveList<T>::Type* newObject )
{
	assert( newObject != nullptr );
	assert( newObject->m_PrevNode == nullptr );
	assert( newObject->m_NextNode == nullptr );

	if( m_TailNode )
	{
		newObject->m_PrevNode = m_TailNode;
		m_TailNode->m_NextNode = newObject;
		m_TailNode = newObject;
	}
	else
	{
		m_HeadNode = newObject;
		m_TailNode = newObject;
	}
	m_Count++;
}
	
template <typename T>
inline void IntrusiveList<T>::pop_back()
{
	assert( m_TailNode != nullptr );
	if( m_TailNode )
	{
		auto prev = m_TailNode->m_PrevNode;
		m_TailNode->m_PrevNode = nullptr;
		m_TailNode->m_NextNode = nullptr;
		if( prev ){ prev->m_NextNode = nullptr; }
		m_TailNode = prev;
		m_Count--;
	}
}
	
template <typename T>
inline void IntrusiveList<T>::push_front( typename IntrusiveList<T>::Type* newObject )
{
	assert( newObject != nullptr );
	assert( newObject->m_PrevNode == nullptr );
	assert( newObject->m_NextNode == nullptr );

	if( m_HeadNode )
	{
		newObject->m_NextNode = m_HeadNode;
		m_HeadNode->m_PrevNode = newObject;
		m_HeadNode = newObject;
	}
	else
	{
		m_HeadNode = newObject;
		m_TailNode = newObject;
	}
	m_Count++;
}
	
template <typename T>
inline void IntrusiveList<T>::pop_front()
{
	assert( m_HeadNode != nullptr );
	if( m_HeadNode )
	{
		auto next = m_HeadNode->m_NextNode;
		m_HeadNode->m_PrevNode = nullptr;
		m_HeadNode->m_NextNode = nullptr;
		if( next ){ next->m_PrevNode = nullptr; }
		m_HeadNode = next;
		m_Count--;
	}
}
	
template <typename T>
inline typename IntrusiveList<T>::Iterator
IntrusiveList<T>::insert( typename IntrusiveList<T>::Iterator it, Type* newObject )
{
	assert( newObject != nullptr );
	assert( newObject->m_PrevNode == nullptr );
	assert( newObject->m_NextNode == nullptr );
	auto prev = it->m_PrevNode;
	newObject->m_PrevNode = prev;
	newObject->m_NextNode = *it;
	if( prev ){ prev->m_NextNode = newObject; }
	else{ m_HeadNode = newObject; }
	m_Count++;
	return IntrusiveList<T>::Iterator( newObject );
}
	
template <typename T>
inline typename IntrusiveList<T>::Iterator 
IntrusiveList<T>::erase( typename IntrusiveList<T>::Iterator it )
{
	auto prev = it->m_PrevNode;
	auto next = it->m_NextNode;
	it->m_PrevNode = nullptr;
	it->m_NextNode = nullptr;
	if( prev ) prev->m_NextNode = next;
	else m_HeadNode = next;
	if( next ){ next->m_PrevNode = prev; }
	else{ m_TailNode = prev; }
	m_Count--;
	return IntrusiveList<T>::Iterator( next );
}
	
template <typename T>
inline void IntrusiveList<T>::clear()
{
	for( Type* it = m_HeadNode; it != nullptr; )
	{
		Type* next = it->m_NextNode;
		it->m_PrevNode = nullptr;
		it->m_NextNode = nullptr;
		it = next;
	}
	m_HeadNode = nullptr;
	m_TailNode = nullptr;
	m_Count = 0;
}

template <typename T>
T* IntrusiveList<T>::front() const
{
	assert( m_HeadNode != nullptr );
	return m_HeadNode;
}

template <typename T>
T* IntrusiveList<T>::back() const
{
	assert( m_TailNode != nullptr );
	return m_TailNode;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INTRUSIVE_LIST_H__

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
class Instance : public IntrusiveList<Instance>::Node
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
	
	// Color for binding
	Color		ColorInheritance;

	// Parent color
	Color		ColorParent;

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
		int32_t		delay;
	} soundValues;

	// 状態
	eInstanceState	m_State;

	// 生存時間
	float		m_LivedTime;

	// 生成されてからの時間
	float		m_LivingTime;

	// The time offset for UV
	int32_t		uvTimeOffset;

	// Scroll, FCurve area for UV
	RectF		uvAreaOffset;

	// Scroll speed for UV
	Vector2D	uvScrollSpeed;

	// The number of generated chiledren. (fixed size)
	int32_t		m_fixedGeneratedChildrenCount[ChildrenMax];

	// The time to generate next child.  (fixed size)
	float		m_fixedNextGenerationTime[ChildrenMax];

	// The number of generated chiledren. (flexible size)
	int32_t*		m_flexibleGeneratedChildrenCount;

	// The time to generate next child.  (flexible size)
	float*		m_flexibleNextGenerationTime;

	// The number of generated chiledren. (actually used)
	int32_t*		m_generatedChildrenCount;

	// The time to generate next child.  (actually used)
	float*			m_nextGenerationTime;

	// Spawning Method matrix
	Matrix43		m_GenerationLocation;

	// 変換用行列
	Matrix43		m_GlobalMatrix43;

	// 親の変換用行列
	Matrix43		m_ParentMatrix43;

	// 変換用行列が計算済かどうか
	bool			m_GlobalMatrix43Calculated;

	// 親の変換用行列が計算済かどうか
	bool			m_ParentMatrix43Calculated;

	/* 時間を進めるかどうか? */
	bool			m_stepTime;

	/* 更新番号 */
	uint32_t		m_sequenceNumber;

	// コンストラクタ
	Instance( Manager* pManager, EffectNode* pEffectNode, InstanceContainer* pContainer );

	// デストラクタ
	virtual ~Instance();

	void GenerateChildrenInRequired(float currentTime);

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
	void Initialize( Instance* parent, int32_t instanceNumber, int32_t parentTime, const Matrix43& globalMatrix);

	/**
		@brief	更新
	*/
	void Update( float deltaFrame, bool shown );

	/**
		@brief	Draw instance
	*/
	void Draw(Instance* next);

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
	void CalculateParentMatrix( float deltaFrame );
	
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
	: public IRandObject
{
	friend class ManagerImplemented;

private:
	/* このエフェクトで使用しているインスタンス数 */
	int			m_instanceCount;
	
	/* 更新されたフレーム数 */
	float		m_updatedFrame;

	InstanceContainer*	m_rootContainer;
	Vector3D			m_targetLocation;

	int32_t				m_seed = 0;

	InstanceGlobal();

	virtual ~InstanceGlobal();

public:

	bool		IsGlobalColorSet = false;
	Color		GlobalColor = Color(255, 255, 255, 255);

	std::vector<InstanceContainer*>	RenderedInstanceContainers;

	void SetSeed(int32_t seed);

	virtual float GetRand() override;

	virtual float GetRand(float min_, float max_) override;

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
	IntrusiveList<Instance> m_instances;
	IntrusiveList<Instance> m_removingInstances;

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

	InstanceGlobal* GetInstanceGlobal() const { return m_global; }

};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_INSTANCEGROUP_H__

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
float FCurve::GetOffset(InstanceGlobal& g) const
{
	return g.GetRand(m_offsetMin, m_offsetMax);
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
FCurveVector2D::FCurveVector2D()
	: X(0)
	, Y(0)
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t FCurveVector2D::Load(void* data, int32_t version)
{
	int32_t size = 0;
	uint8_t* p = (uint8_t*) data;

	int32_t x_size = X.Load(p, version);
	size += x_size;
	p += x_size;

	int32_t y_size = Y.Load(p, version);
	size += y_size;
	p += y_size;

	return size;
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
	: m_effect(effect)
	, m_userData(NULL)
	, IsRendered(true)
	, TranslationFCurve(NULL)
	, RotationFCurve(NULL)
	, ScalingFCurve(NULL)
	, SoundType(ParameterSoundType_None)
	, RenderingOrder(RenderingOrder_FirstCreatedInstanceIsFirst)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::LoadParameter(unsigned char*& pos, EffectNode* parent, Setting* setting)
{
	int size = 0;
	int node_type = 0;
	auto ef = (EffectImplemented*)m_effect;

	memcpy(&node_type, pos, sizeof(int));
	pos += sizeof(int);

	if (node_type == -1)
	{
		TranslationType = ParameterTranslationType_None;
		LocationAbs.type = LocationAbsType::None;
		RotationType = ParameterRotationType_None;
		ScalingType = ParameterScalingType_None;
		CommonValues.MaxGeneration = 1;

		GenerationLocation.EffectsRotation = 0;
		GenerationLocation.type = ParameterGenerationLocation::TYPE_POINT;
		GenerationLocation.point.location.reset();

		RenderingPriority = -1;
	}
	else
	{
		if (m_effect->GetVersion() >= 10)
		{
			int32_t rendered = 0;
			memcpy(&rendered, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			IsRendered = rendered != 0;
		}

		// To render with priority, nodes are assigned a list.
		if (m_effect->GetVersion() >= 13)
		{
			memcpy(&RenderingPriority, pos, sizeof(int32_t));
			pos += sizeof(int32_t);
		}
		else
		{
			RenderingPriority = -1;
		}

		memcpy(&size, pos, sizeof(int));
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

		memcpy(&TranslationType, pos, sizeof(int));
		pos += sizeof(int);

		if (TranslationType == ParameterTranslationType_Fixed)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterTranslationFixed));
			memcpy(&TranslationFixed, pos, size);
			pos += size;

			// 無効化
			if (TranslationFixed.Position.X == 0.0f &&
				TranslationFixed.Position.Y == 0.0f &&
				TranslationFixed.Position.Z == 0.0f)
			{
				TranslationType = ParameterTranslationType_None;
				EffekseerPrintDebug("LocationType Change None\n");
			}
		}
		else if (TranslationType == ParameterTranslationType_PVA)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterTranslationPVA));
			memcpy(&TranslationPVA, pos, size);
			pos += size;
		}
		else if (TranslationType == ParameterTranslationType_Easing)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(easing_vector3d));
			memcpy(&TranslationEasing, pos, size);
			pos += size;
		}
		else if (TranslationType == ParameterTranslationType_FCurve)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);

			TranslationFCurve = new FCurveVector3D();
			pos += TranslationFCurve->Load(pos, m_effect->GetVersion());
		}

		/* 位置拡大処理 */
		if (ef->IsDyanamicMagnificationValid())
		{
			if (TranslationType == ParameterTranslationType_Fixed)
			{
				TranslationFixed.Position *= m_effect->GetMaginification();
			}
			else if (TranslationType == ParameterTranslationType_PVA)
			{
				TranslationPVA.location.min *= m_effect->GetMaginification();
				TranslationPVA.location.max *= m_effect->GetMaginification();
				TranslationPVA.velocity.min *= m_effect->GetMaginification();
				TranslationPVA.velocity.max *= m_effect->GetMaginification();
				TranslationPVA.acceleration.min *= m_effect->GetMaginification();
				TranslationPVA.acceleration.max *= m_effect->GetMaginification();
			}
			else if (TranslationType == ParameterTranslationType_Easing)
			{
				TranslationEasing.start.min *= m_effect->GetMaginification();
				TranslationEasing.start.max *= m_effect->GetMaginification();
				TranslationEasing.end.min *= m_effect->GetMaginification();
				TranslationEasing.end.max *= m_effect->GetMaginification();
			}
			else if (TranslationType == ParameterTranslationType_FCurve)
			{
				TranslationFCurve->X.Maginify(m_effect->GetMaginification());
				TranslationFCurve->Y.Maginify(m_effect->GetMaginification());
				TranslationFCurve->Z.Maginify(m_effect->GetMaginification());
			}
		}

		memcpy(&LocationAbs.type, pos, sizeof(int));
		pos += sizeof(int);

		// Calc attraction forces
		if (LocationAbs.type == LocationAbsType::None)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == 0);
			memcpy(&LocationAbs.none, pos, size);
			pos += size;
		}
		else if (LocationAbs.type == LocationAbsType::Gravity)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(vector3d));
			memcpy(&LocationAbs.gravity, pos, size);
			pos += size;
		}
		else if (LocationAbs.type == LocationAbsType::AttractiveForce)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(LocationAbs.attractiveForce));
			memcpy(&LocationAbs.attractiveForce, pos, size);
			pos += size;
		}

		// Magnify attraction forces
		if (ef->IsDyanamicMagnificationValid())
		{
			if (LocationAbs.type == LocationAbsType::None)
			{
			}
			else if (LocationAbs.type == LocationAbsType::Gravity)
			{
				LocationAbs.gravity *= m_effect->GetMaginification();
			}
			else if (LocationAbs.type == LocationAbsType::AttractiveForce)
			{
				LocationAbs.attractiveForce.control *= m_effect->GetMaginification();
				LocationAbs.attractiveForce.force *= m_effect->GetMaginification();
				LocationAbs.attractiveForce.minRange *= m_effect->GetMaginification();
				LocationAbs.attractiveForce.maxRange *= m_effect->GetMaginification();
			}
		}

		memcpy(&RotationType, pos, sizeof(int));
		pos += sizeof(int);
		EffekseerPrintDebug("RotationType %d\n", RotationType);
		if (RotationType == ParameterRotationType_Fixed)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterRotationFixed));
			memcpy(&RotationFixed, pos, size);
			pos += size;

			// 無効化
			if (RotationFixed.Position.X == 0.0f &&
				RotationFixed.Position.Y == 0.0f &&
				RotationFixed.Position.Z == 0.0f)
			{
				RotationType = ParameterRotationType_None;
				EffekseerPrintDebug("RotationType Change None\n");
			}
		}
		else if (RotationType == ParameterRotationType_PVA)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterRotationPVA));
			memcpy(&RotationPVA, pos, size);
			pos += size;
		}
		else if (RotationType == ParameterRotationType_Easing)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(easing_vector3d));
			memcpy(&RotationEasing, pos, size);
			pos += size;
		}
		else if (RotationType == ParameterRotationType_AxisPVA)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterRotationAxisPVA));
			memcpy(&RotationAxisPVA, pos, size);
			pos += size;
		}
		else if (RotationType == ParameterRotationType_AxisEasing)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterRotationAxisEasing));
			memcpy(&RotationAxisEasing, pos, size);
			pos += size;
		}
		else if (RotationType == ParameterRotationType_FCurve)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);

			RotationFCurve = new FCurveVector3D();
			pos += RotationFCurve->Load(pos, m_effect->GetVersion());
		}

		memcpy(&ScalingType, pos, sizeof(int));
		pos += sizeof(int);
		EffekseerPrintDebug("ScalingType %d\n", ScalingType);
		if (ScalingType == ParameterScalingType_Fixed)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterScalingFixed));
			memcpy(&ScalingFixed, pos, size);
			pos += size;

			// 無効化
			if (ScalingFixed.Position.X == 1.0f &&
				ScalingFixed.Position.Y == 1.0f &&
				ScalingFixed.Position.Z == 1.0f)
			{
				ScalingType = ParameterScalingType_None;
				EffekseerPrintDebug("ScalingType Change None\n");
			}
		}
		else if (ScalingType == ParameterScalingType_PVA)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterScalingPVA));
			memcpy(&ScalingPVA, pos, size);
			pos += size;
		}
		else if (ScalingType == ParameterScalingType_Easing)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(easing_vector3d));
			memcpy(&ScalingEasing, pos, size);
			pos += size;
		}
		else if (ScalingType == ParameterScalingType_SinglePVA)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(ParameterScalingSinglePVA));
			memcpy(&ScalingSinglePVA, pos, size);
			pos += size;
		}
		else if (ScalingType == ParameterScalingType_SingleEasing)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);
			assert(size == sizeof(easing_float));
			memcpy(&ScalingSingleEasing, pos, size);
			pos += size;
		}
		else if (ScalingType == ParameterScalingType_FCurve)
		{
			memcpy(&size, pos, sizeof(int));
			pos += sizeof(int);

			ScalingFCurve = new FCurveVector3D();
			pos += ScalingFCurve->Load(pos, m_effect->GetVersion());
			ScalingFCurve->X.SetDefaultValue(1.0f);
			ScalingFCurve->Y.SetDefaultValue(1.0f);
			ScalingFCurve->Z.SetDefaultValue(1.0f);
		}

		/* Spawning Method */
		GenerationLocation.load(pos, m_effect->GetVersion());

		/* Spawning Method 拡大処理*/
		if (ef->IsDyanamicMagnificationValid()
			/* && (this->CommonValues.ScalingBindType == BindType::NotBind || parent->GetType() == EFFECT_NODE_TYPE_ROOT)*/)
		{
			if (GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT)
			{
				GenerationLocation.point.location.min *= m_effect->GetMaginification();
				GenerationLocation.point.location.max *= m_effect->GetMaginification();
			}
			else if (GenerationLocation.type == ParameterGenerationLocation::TYPE_LINE)
			{
				GenerationLocation.line.position_end.min *= m_effect->GetMaginification();
				GenerationLocation.line.position_end.max *= m_effect->GetMaginification();
				GenerationLocation.line.position_start.min *= m_effect->GetMaginification();
				GenerationLocation.line.position_start.max *= m_effect->GetMaginification();
				GenerationLocation.line.position_noize.min *= m_effect->GetMaginification();
				GenerationLocation.line.position_noize.max *= m_effect->GetMaginification();
			}
			else if (GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE)
			{
				GenerationLocation.sphere.radius.min *= m_effect->GetMaginification();
				GenerationLocation.sphere.radius.max *= m_effect->GetMaginification();
			}
			else if (GenerationLocation.type == ParameterGenerationLocation::TYPE_CIRCLE)
			{
				GenerationLocation.circle.radius.min *= m_effect->GetMaginification();
				GenerationLocation.circle.radius.max *= m_effect->GetMaginification();
			}
		}

		// Load depth values
		if (m_effect->GetVersion() >= 12)
		{
			memcpy(&DepthValues.DepthOffset, pos, sizeof(float));
			pos += sizeof(float);

			auto IsDepthOffsetScaledWithCamera = 0;
			memcpy(&IsDepthOffsetScaledWithCamera, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			DepthValues.IsDepthOffsetScaledWithCamera = IsDepthOffsetScaledWithCamera > 0;

			auto IsDepthOffsetScaledWithParticleScale = 0;
			memcpy(&IsDepthOffsetScaledWithParticleScale, pos, sizeof(int32_t));
			pos += sizeof(int32_t);

			DepthValues.IsDepthOffsetScaledWithParticleScale = IsDepthOffsetScaledWithParticleScale > 0;

			if (m_effect->GetVersion() >= 13)
			{
				memcpy(&DepthValues.ZSort, pos, sizeof(int32_t));
				pos += sizeof(int32_t);
				memcpy(&DepthValues.DrawingPriority, pos, sizeof(int32_t));
				pos += sizeof(int32_t);
			}

			memcpy(&DepthValues.SoftParticle, pos, sizeof(float));
			pos += sizeof(float);

			DepthValues.DepthOffset *= m_effect->GetMaginification();
			DepthValues.SoftParticle *= m_effect->GetMaginification();
		}

		// Convert right handle coordinate system into left handle coordinate system
		if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
		{
			// Translation
			if (TranslationType == ParameterTranslationType_Fixed)
			{
				TranslationFixed.Position.Z *= -1.0f;
			}
			else if (TranslationType == ParameterTranslationType_PVA)
			{
				TranslationPVA.location.max.z *= -1.0f;
				TranslationPVA.location.min.z *= -1.0f;
				TranslationPVA.velocity.max.z *= -1.0f;
				TranslationPVA.velocity.min.z *= -1.0f;
				TranslationPVA.acceleration.max.z *= -1.0f;
				TranslationPVA.acceleration.min.z *= -1.0f;
			}
			else if (TranslationType == ParameterTranslationType_Easing)
			{
				TranslationEasing.start.max.z *= -1.0f;
				TranslationEasing.start.min.z *= -1.0f;
				TranslationEasing.end.max.z *= -1.0f;
				TranslationEasing.end.min.z *= -1.0f;
			}

			// Rotation
			if (RotationType == ParameterRotationType_Fixed)
			{
				RotationFixed.Position.X *= -1.0f;
				RotationFixed.Position.Y *= -1.0f;
			}
			else if (RotationType == ParameterRotationType_PVA)
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
			else if (RotationType == ParameterRotationType_Easing)
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
			else if (RotationType == ParameterRotationType_AxisPVA)
			{
				RotationAxisPVA.axis.max.z *= -1.0f;
				RotationAxisPVA.axis.min.z *= -1.0f;
			}
			else if (RotationType == ParameterRotationType_AxisEasing)
			{
				RotationAxisEasing.axis.max.z *= -1.0f;
				RotationAxisEasing.axis.min.z *= -1.0f;
			}
			else if (RotationType == ParameterRotationType_FCurve)
			{
				RotationFCurve->X.ChangeCoordinate();
				RotationFCurve->Y.ChangeCoordinate();
			}

			// GenerationLocation
			if (GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT)
			{

			}
			else if (GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE)
			{
				GenerationLocation.sphere.rotation_x.max *= -1.0f;
				GenerationLocation.sphere.rotation_x.min *= -1.0f;
				GenerationLocation.sphere.rotation_y.max *= -1.0f;
				GenerationLocation.sphere.rotation_y.min *= -1.0f;
			}
		}

		if (m_effect->GetVersion() >= 3)
		{
			RendererCommon.load(pos, m_effect->GetVersion());
		}
		else
		{
			RendererCommon.reset();
		}

		LoadRendererParameter(pos, m_effect->GetSetting());

		if (m_effect->GetVersion() >= 1)
		{
			// Sound
			memcpy(&SoundType, pos, sizeof(int));
			pos += sizeof(int);
			if (SoundType == ParameterSoundType_Use)
			{
				memcpy(&Sound.WaveId, pos, sizeof(int32_t));
				pos += sizeof(int32_t);
				memcpy(&Sound.Volume, pos, sizeof(random_float));
				pos += sizeof(random_float);
				memcpy(&Sound.Pitch, pos, sizeof(random_float));
				pos += sizeof(random_float);
				memcpy(&Sound.PanType, pos, sizeof(ParameterSoundPanType));
				pos += sizeof(ParameterSoundPanType);
				memcpy(&Sound.Pan, pos, sizeof(random_float));
				pos += sizeof(random_float);
				memcpy(&Sound.Distance, pos, sizeof(float));
				pos += sizeof(float);
				memcpy(&Sound.Delay, pos, sizeof(random_int));
				pos += sizeof(random_int);
			}
		}
	}

	// ノード
	int nodeCount = 0;
	memcpy(&nodeCount, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("ChildrenCount : %d\n", nodeCount);
	m_Nodes.resize(nodeCount);
	for (size_t i = 0; i < m_Nodes.size(); i++)
	{
		m_Nodes[i] = EffectNodeImplemented::Create(m_effect, this, pos);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNodeImplemented::~EffectNodeImplemented()
{
	for (size_t i = 0; i < m_Nodes.size(); i++)
	{
		ES_SAFE_DELETE(m_Nodes[i]);
	}

	ES_SAFE_DELETE(TranslationFCurve);
	ES_SAFE_DELETE(RotationFCurve);
	ES_SAFE_DELETE(ScalingFCurve);
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
	param.ColorTextureIndex = RendererCommon.ColorTextureIndex;
	param.AlphaBlend = RendererCommon.AlphaBlend;
	param.Distortion = RendererCommon.Distortion;
	param.DistortionIntensity = RendererCommon.DistortionIntensity;
	param.FilterType = RendererCommon.FilterType;
	param.WrapType = RendererCommon.WrapType;
	param.ZTest = RendererCommon.ZTest;
	param.ZWrite = RendererCommon.ZWrite;
	return param;
}

void EffectNodeImplemented::SetBasicRenderParameter(EffectBasicRenderParameter param)
{
	RendererCommon.ColorTextureIndex = param.ColorTextureIndex;
	RendererCommon.AlphaBlend = param.AlphaBlend;
	RendererCommon.Distortion = param.Distortion;
	RendererCommon.DistortionIntensity = param.DistortionIntensity;
	RendererCommon.FilterType = param.FilterType;
	RendererCommon.WrapType = param.WrapType;
	RendererCommon.ZTest = param.ZTest;
	RendererCommon.ZWrite = param.ZWrite;
}

EffectModelParameter EffectNodeImplemented::GetEffectModelParameter()
{
	EffectModelParameter param;
	param.Lighting = false;

	if (GetType() == EFFECT_NODE_TYPE_MODEL)
	{
		auto t = (EffectNodeModel*)this;
		param.Lighting = t->Lighting;
	}

	return param;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::LoadRendererParameter(unsigned char*& pos, Setting* setting)
{
	int32_t type = 0;
	memcpy(&type, pos, sizeof(int));
	pos += sizeof(int);
	assert(type == GetType());
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

void EffectNodeImplemented::EndRenderingGroup(InstanceGroup* group, Manager* manager)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
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

	if (RendererCommon.FadeInType == ParameterRendererCommon::FADEIN_ON && instance.m_LivingTime < RendererCommon.FadeIn.Frame)
	{
		float v = 1.0f;
		RendererCommon.FadeIn.Value.setValueToArg(
			v,
			0.0f,
			1.0f,
			(float)instance.m_LivingTime / (float)RendererCommon.FadeIn.Frame);

		alpha *= v;
	}

	if (RendererCommon.FadeOutType == ParameterRendererCommon::FADEOUT_ON && instance.m_LivingTime + RendererCommon.FadeOut.Frame > instance.m_LivedTime)
	{
		float v = 1.0f;
		RendererCommon.FadeOut.Value.setValueToArg(
			v,
			1.0f,
			0.0f,
			(float)(instance.m_LivingTime + RendererCommon.FadeOut.Frame - instance.m_LivedTime) / (float)RendererCommon.FadeOut.Frame);

		alpha *= v;
	}

	return alpha;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeImplemented::PlaySound_(Instance& instance, SoundTag tag, Manager* manager)
{
	auto instanceGlobal = instance.m_pContainer->GetRootInstance();

	SoundPlayer* player = manager->GetSoundPlayer();
	if (player == NULL)
	{
		return;
	}

	if (Sound.WaveId >= 0)
	{
		SoundPlayer::InstanceParameter parameter;
		parameter.Data = m_effect->GetWave(Sound.WaveId);
		parameter.Volume = Sound.Volume.getValue(*instanceGlobal);
		parameter.Pitch = Sound.Pitch.getValue(*instanceGlobal);
		parameter.Pan = Sound.Pan.getValue(*instanceGlobal);

		parameter.Mode3D = (Sound.PanType == ParameterSoundPanType_3D);
		Vector3D::Transform(parameter.Position,
			Vector3D(0.0f, 0.0f, 0.0f), instance.GetGlobalMatrix43());
		parameter.Distance = Sound.Distance;

		player->Play(tag, parameter);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectNodeImplemented* EffectNodeImplemented::Create(Effect* effect, EffectNode* parent, unsigned char*& pos)
{
	EffectNodeImplemented* effectnode = NULL;

	int node_type = 0;
	memcpy(&node_type, pos, sizeof(int));

	if (node_type == EFFECT_NODE_TYPE_ROOT)
	{
		EffekseerPrintDebug("* Create : EffectNodeRoot\n");
		effectnode = new EffectNodeRoot(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_NONE)
	{
		EffekseerPrintDebug("* Create : EffectNodeNone\n");
		effectnode = new EffectNodeImplemented(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_SPRITE)
	{
		EffekseerPrintDebug("* Create : EffectNodeSprite\n");
		effectnode = new EffectNodeSprite(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_RIBBON)
	{
		EffekseerPrintDebug("* Create : EffectNodeRibbon\n");
		effectnode = new EffectNodeRibbon(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_RING)
	{
		EffekseerPrintDebug("* Create : EffectNodeRing\n");
		effectnode = new EffectNodeRing(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_MODEL)
	{
		EffekseerPrintDebug("* Create : EffectNodeModel\n");
		effectnode = new EffectNodeModel(effect, pos);
	}
	else if (node_type == EFFECT_NODE_TYPE_TRACK)
	{
		EffekseerPrintDebug("* Create : EffectNodeTrack\n");
		effectnode = new EffectNodeTrack(effect, pos);
	}
	else
	{
		assert(0);
	}

	effectnode->LoadParameter(pos, parent, effect->GetSetting());

	return effectnode;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

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

	AlphaBlend = RendererCommon.AlphaBlend;

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

	if (m_effect->GetVersion() >= 12)
	{
		memcpy(&Billboard, pos, sizeof(int));
		pos += sizeof(int);
	}
	else
	{
		Billboard = BillboardType::Fixed;
	}

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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = RendererCommon.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Billboard = Billboard;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		renderer->BeginRendering(nodeParameter, count, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.model;
	ModelRenderer* renderer = manager->GetModelRenderer();
	if( renderer != NULL )
	{
		ModelRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = RendererCommon.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Billboard = Billboard;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		ModelRenderer::InstanceParameter instanceParameter;
		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();
		instanceParameter.Time = (int32_t)instance.m_LivingTime;

		instanceParameter.UV = instance.GetUV();
		
		Color _color;
		if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
		{
			_color = Color::Mul(instValues._original, instance.ColorParent);
		}
		else
		{
			_color = instValues._original;
		}
		instanceParameter.AllColor = _color;
		
		if (instance.m_pContainer->GetRootInstance()->IsGlobalColorSet)
		{
			instanceParameter.AllColor = Color::Mul(instanceParameter.AllColor, instance.m_pContainer->GetRootInstance()->GlobalColor);
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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.ModelIndex = ModelIndex;
		nodeParameter.ColorTextureIndex = RendererCommon.ColorTextureIndex;
		nodeParameter.Culling = Culling;
		nodeParameter.Billboard = Billboard;
		nodeParameter.Lighting = Lighting;
		nodeParameter.NormalTextureIndex = NormalTextureIndex;
		nodeParameter.Magnification = m_effect->GetMaginification();
		nodeParameter.IsRightHand = manager->GetSetting()->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.model;
	auto instanceGlobal = instance.m_pContainer->GetRootInstance();

	if( AllColor.type == StandardColorParameter::Fixed )
	{
		instValues._original = AllColor.fixed.all;
		instValues.allColorValues.fixed._color = instValues._original;
	}
	else if( AllColor.type == StandardColorParameter::Random )
	{
		instValues._original = AllColor.random.all.getValue(*instanceGlobal);
		instValues.allColorValues.random._color = instValues._original;
	}
	else if( AllColor.type == StandardColorParameter::Easing )
	{
		instValues.allColorValues.easing.start = AllColor.easing.all.getStartValue(*instanceGlobal);
		instValues.allColorValues.easing.end = AllColor.easing.all.getEndValue(*instanceGlobal);

		float t = instance.m_LivingTime / instance.m_LivedTime;

		AllColor.easing.all.setValueToArg(
			instValues._original,
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( AllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues.allColorValues.fcurve_rgba.offset[0] = AllColor.fcurve_rgba.FCurve->R.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[1] = AllColor.fcurve_rgba.FCurve->G.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[2] = AllColor.fcurve_rgba.FCurve->B.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[3] = AllColor.fcurve_rgba.FCurve->A.GetOffset(*instanceGlobal);
		
		instValues._original.R = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + AllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.G = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + AllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.B = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + AllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.A = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + AllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._original, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._original;
	}

	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeModel::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.model;

	if (AllColor.type == StandardColorParameter::Fixed)
	{
		instValues._original = instValues.allColorValues.fixed._color;
	}
	else if (AllColor.type == StandardColorParameter::Random)
	{
		instValues._original = instValues.allColorValues.random._color;
	}
	else if( AllColor.type == StandardColorParameter::Easing )
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		AllColor.easing.all.setValueToArg(
			instValues._original,
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( AllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues._original.R = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + AllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.G = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + AllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.B = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + AllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._original.A = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + AllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}

	float fadeAlpha = GetFadeAlpha(instance);
	if (fadeAlpha != 1.0f)
	{
		instValues._original.A = (uint8_t)(instValues._original.A * fadeAlpha);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._original, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._original;
	}

	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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
	memcpy(&type, pos, sizeof(int));
	pos += sizeof(int);
	assert(type == GetType());
	EffekseerPrintDebug("Renderer : Ribbon\n");

	if (m_effect->GetVersion() >= 3)
	{
		AlphaBlend = RendererCommon.AlphaBlend;
	}
	else
	{
		memcpy(&AlphaBlend, pos, sizeof(int));
		pos += sizeof(int);
	}

	memcpy(&ViewpointDependent, pos, sizeof(int));
	pos += sizeof(int);

	memcpy(&RibbonAllColor.type, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonColorAllType : %d\n", RibbonAllColor.type);

	if (RibbonAllColor.type == RibbonAllColorParameter::Fixed)
	{
		memcpy(&RibbonAllColor.fixed, pos, sizeof(RibbonAllColor.fixed));
		pos += sizeof(RibbonAllColor.fixed);
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Random)
	{
		RibbonAllColor.random.all.load(m_effect->GetVersion(), pos);
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Easing)
	{
		RibbonAllColor.easing.all.load(m_effect->GetVersion(), pos);
	}

	memcpy(&RibbonColor.type, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonColorType : %d\n", RibbonColor.type);

	if (RibbonColor.type == RibbonColor.Default)
	{
	}
	else if (RibbonColor.type == RibbonColor.Fixed)
	{
		memcpy(&RibbonColor.fixed, pos, sizeof(RibbonColor.fixed));
		pos += sizeof(RibbonColor.fixed);
	}

	memcpy(&RibbonPosition.type, pos, sizeof(int));
	pos += sizeof(int);
	EffekseerPrintDebug("RibbonPosition : %d\n", RibbonPosition.type);

	if (RibbonPosition.type == RibbonPosition.Default)
	{
		if (m_effect->GetVersion() >= 8)
		{
			memcpy(&RibbonPosition.fixed, pos, sizeof(RibbonPosition.fixed));
			pos += sizeof(RibbonPosition.fixed);
			RibbonPosition.type = RibbonPosition.Fixed;
		}
	}
	else if (RibbonPosition.type == RibbonPosition.Fixed)
	{
		memcpy(&RibbonPosition.fixed, pos, sizeof(RibbonPosition.fixed));
		pos += sizeof(RibbonPosition.fixed);
	}

	if (m_effect->GetVersion() >= 13)
	{
		memcpy(&SplineDivision, pos, sizeof(int32_t));
		pos += sizeof(int32_t);
	}

	if (m_effect->GetVersion() >= 3)
	{
		RibbonTexture = RendererCommon.ColorTextureIndex;
	}
	else
	{
		memcpy(&RibbonTexture, pos, sizeof(int));
		pos += sizeof(int);
	}

	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
	}

	/* 位置拡大処理 */
	if (m_effect->GetVersion() >= 8)
	{
		if (RibbonPosition.type == RibbonPosition.Default)
		{
		}
		else if (RibbonPosition.type == RibbonPosition.Fixed)
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
	if (renderer != NULL)
	{
		m_nodeParameter.AlphaBlend = AlphaBlend;
		m_nodeParameter.TextureFilter = RendererCommon.FilterType;
		m_nodeParameter.TextureWrap = RendererCommon.WrapType;
		m_nodeParameter.ZTest = RendererCommon.ZTest;
		m_nodeParameter.ZWrite = RendererCommon.ZWrite;
		m_nodeParameter.ViewpointDependent = ViewpointDependent != 0;
		m_nodeParameter.ColorTextureIndex = RibbonTexture;
		m_nodeParameter.EffectPointer = GetEffect();

		m_nodeParameter.Distortion = RendererCommon.Distortion;
		m_nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		m_nodeParameter.SplineDivision = SplineDivision;

		renderer->BeginRendering(m_nodeParameter, count, m_userData);
	}
}

void EffectNodeRibbon::BeginRenderingGroup(InstanceGroup* group, Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if (renderer != NULL)
	{
		m_instanceParameter.InstanceCount = group->GetInstanceCount();
		m_instanceParameter.InstanceIndex = 0;

		if (group->GetFirst() != nullptr)
		{
			m_instanceParameter.UV = group->GetFirst()->GetUV();
		}

		renderer->BeginRenderingGroup(m_nodeParameter, m_instanceParameter.InstanceCount, m_userData);
	}
}

void EffectNodeRibbon::EndRenderingGroup(InstanceGroup* group, Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if (renderer != NULL)
	{
		renderer->EndRenderingGroup(m_nodeParameter, m_instanceParameter.InstanceCount, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.ribbon;
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if (renderer != NULL)
	{
		Color _color;
		if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
		{
			_color = Color::Mul(instValues._original, instance.ColorParent);
		}
		else
		{
			_color = instValues._original;
		}

		m_instanceParameter.AllColor = _color;
		m_instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		Color color_l = _color;
		Color color_r = _color;
		Color color_nl = _color;
		Color color_nr = _color;

		if (next_instance != nullptr)
		{
			const InstanceValues& instValues_next = next_instance->rendererValues.ribbon;
			Color _color_next;
			if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
			{
				_color_next = Color::Mul(instValues_next._original, next_instance->ColorParent);
			}
			else
			{
				_color_next = instValues_next._original;
			}

			color_nl = _color_next;
			color_nr = _color_next;
		}

		if (RibbonColor.type == RibbonColorParameter::Default)
		{

		}
		else if (RibbonColor.type == RibbonColorParameter::Fixed)
		{
			color_l = Color::Mul(color_l, RibbonColor.fixed.l);
			color_r = Color::Mul(color_r, RibbonColor.fixed.r);
			color_nl = Color::Mul(color_nl, RibbonColor.fixed.l);
			color_nr = Color::Mul(color_nr, RibbonColor.fixed.r);
		}

		m_instanceParameter.Colors[0] = color_l;
		m_instanceParameter.Colors[1] = color_r;
		m_instanceParameter.Colors[2] = color_nl;
		m_instanceParameter.Colors[3] = color_nr;

		// Apply global Color
		if (instance.m_pContainer->GetRootInstance()->IsGlobalColorSet)
		{
			m_instanceParameter.Colors[0] = Color::Mul(m_instanceParameter.Colors[0], instance.m_pContainer->GetRootInstance()->GlobalColor);
			m_instanceParameter.Colors[1] = Color::Mul(m_instanceParameter.Colors[1], instance.m_pContainer->GetRootInstance()->GlobalColor);
		}

		if (RibbonPosition.type == RibbonPositionParameter::Default)
		{
			m_instanceParameter.Positions[0] = -0.5f;
			m_instanceParameter.Positions[1] = 0.5f;
		}
		else if (RibbonPosition.type == RibbonPositionParameter::Fixed)
		{
			m_instanceParameter.Positions[0] = RibbonPosition.fixed.l;
			m_instanceParameter.Positions[1] = RibbonPosition.fixed.r;
		}

		renderer->Rendering(m_nodeParameter, m_instanceParameter, m_userData);

		m_instanceParameter.InstanceIndex++;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::EndRendering(Manager* manager)
{
	RibbonRenderer* renderer = manager->GetRibbonRenderer();
	if (renderer != NULL)
	{
		renderer->EndRendering(m_nodeParameter, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ribbon;
	auto instanceGlobal = instance.m_pContainer->GetRootInstance();

	if (RibbonAllColor.type == RibbonAllColorParameter::Fixed)
	{
		instValues._original = RibbonAllColor.fixed.all;
		instValues.allColorValues.fixed._color = instValues._original;
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Random)
	{
		instValues._original = RibbonAllColor.random.all.getValue(*instanceGlobal);
		instValues.allColorValues.random._color = instValues._original;
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Easing)
	{
		instValues.allColorValues.easing.start = RibbonAllColor.easing.all.getStartValue(*instanceGlobal);
		instValues.allColorValues.easing.end = RibbonAllColor.easing.all.getEndValue(*instanceGlobal);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._original, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._original;
	}

	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRibbon::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.ribbon;

	if (RibbonAllColor.type == RibbonAllColorParameter::Fixed)
	{
		instValues._original = instValues.allColorValues.fixed._color;
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Random)
	{
		instValues._original = instValues.allColorValues.random._color;
	}
	else if (RibbonAllColor.type == RibbonAllColorParameter::Easing)
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		RibbonAllColor.easing.all.setValueToArg(
			instValues._original,
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t);
	}

	float fadeAlpha = GetFadeAlpha(instance);
	if (fadeAlpha != 1.0f)
	{
		instValues._original.A = (uint8_t)(instValues._original.A * fadeAlpha);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._original, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._original;
	}

	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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

	memcpy( &RenderingOrder, pos, sizeof(int) );
	pos += sizeof(int);

	if( m_effect->GetVersion() >= 3)
	{
		AlphaBlend = RendererCommon.AlphaBlend;
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
		RingTexture = RendererCommon.ColorTextureIndex;
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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = RingTexture;
		nodeParameter.VertexCount = VertexCount;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		renderer->BeginRendering( nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.ring;
	RingRenderer* renderer = manager->GetRingRenderer();
	if( renderer != NULL )
	{
		RingRenderer::NodeParameter nodeParameter;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.VertexCount = VertexCount;
		nodeParameter.ColorTextureIndex = RingTexture;
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		Color _outerColor;
		Color _centerColor;
		Color _innerColor;

		if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
		{
			_outerColor = Color::Mul(instValues.outerColor.original, instance.ColorParent);
			_centerColor = Color::Mul(instValues.centerColor.original, instance.ColorParent);
			_innerColor = Color::Mul(instValues.innerColor.original, instance.ColorParent);
		}
		else
		{
			_outerColor = instValues.outerColor.original;
			_centerColor = instValues.centerColor.original;
			_innerColor = instValues.innerColor.original;
		}

		RingRenderer::InstanceParameter instanceParameter;
		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		instanceParameter.ViewingAngle = instValues.viewingAngle.current;
		
		instValues.outerLocation.current.setValueToArg( instanceParameter.OuterLocation );
		instValues.innerLocation.current.setValueToArg( instanceParameter.InnerLocation );

		instanceParameter.CenterRatio = instValues.centerRatio.current;

		// Apply global Color
		if (instance.m_pContainer->GetRootInstance()->IsGlobalColorSet)
		{
			Color::Mul(_outerColor, instance.m_pContainer->GetRootInstance()->GlobalColor);
			Color::Mul(_centerColor, instance.m_pContainer->GetRootInstance()->GlobalColor);
			Color::Mul(_innerColor, instance.m_pContainer->GetRootInstance()->GlobalColor);
		}

		instanceParameter.OuterColor  = _outerColor;
		instanceParameter.CenterColor = _centerColor;
		instanceParameter.InnerColor  = _innerColor;
		
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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = RingTexture;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	auto instanceGlobal = instance.m_pContainer->GetRootInstance();

	InstanceValues& instValues = instance.rendererValues.ring;

	InitializeSingleValues(ViewingAngle, instValues.viewingAngle, manager, instanceGlobal);

	InitializeLocationValues(OuterLocation, instValues.outerLocation, manager, instanceGlobal);
	InitializeLocationValues(InnerLocation, instValues.innerLocation, manager, instanceGlobal);
	
	InitializeSingleValues(CenterRatio, instValues.centerRatio, manager, instanceGlobal);

	InitializeColorValues(OuterColor, instValues.outerColor, manager, instanceGlobal);
	InitializeColorValues(CenterColor, instValues.centerColor, manager, instanceGlobal);
	InitializeColorValues(InnerColor, instValues.innerColor, manager, instanceGlobal);

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues.outerColor.current = Color::Mul(instValues.outerColor.original, instance.ColorParent);
		instValues.centerColor.current = Color::Mul(instValues.centerColor.original, instance.ColorParent);
		instValues.innerColor.current = Color::Mul(instValues.innerColor.original, instance.ColorParent);
	}
	else
	{
		instValues.outerColor.current = instValues.outerColor.original;
		instValues.centerColor.current = instValues.centerColor.original;
		instValues.innerColor.current = instValues.innerColor.original;
	}

	instance.ColorInheritance = instValues.centerColor.current;
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

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues.outerColor.current = Color::Mul(instValues.outerColor.original, instance.ColorParent);
		instValues.centerColor.current = Color::Mul(instValues.centerColor.original, instance.ColorParent);
		instValues.innerColor.current = Color::Mul(instValues.innerColor.original, instance.ColorParent);
	}
	else
	{
		instValues.outerColor.current = instValues.outerColor.original;
		instValues.centerColor.current = instValues.centerColor.original;
		instValues.innerColor.current = instValues.innerColor.original;
	}

	instance.ColorInheritance = instValues.centerColor.current;
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
void EffectNodeRing::InitializeSingleValues(const RingSingleParameter& param, RingSingleValues& values, Manager* manager, InstanceGlobal* instanceGlobal)
{
	switch( param.type )
	{
		case RingSingleParameter::Fixed:
			values.current = param.fixed;
			break;
		case RingSingleParameter::Random:
			values.current = param.random.getValue(*instanceGlobal);
			break;
		case RingSingleParameter::Easing:
			values.easing.start = param.easing.start.getValue(*instanceGlobal);
			values.easing.end = param.easing.end.getValue(*instanceGlobal);
			values.current = values.easing.start;
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeLocationValues(const RingLocationParameter& param, RingLocationValues& values, Manager* manager, InstanceGlobal* instanceGlobal)
{
	switch( param.type )
	{
		case RingLocationParameter::Fixed:
			values.current = param.fixed.location;
			break;
		case RingLocationParameter::PVA:
			values.pva.start = param.pva.location.getValue(*instanceGlobal);
			values.pva.velocity = param.pva.velocity.getValue(*instanceGlobal);
			values.pva.acceleration = param.pva.acceleration.getValue(*instanceGlobal);
			values.current = values.pva.start;
			break;
		case RingLocationParameter::Easing:
			values.easing.start = param.easing.start.getValue(*instanceGlobal);
			values.easing.end = param.easing.end.getValue(*instanceGlobal);
			values.current = values.easing.start;
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeRing::InitializeColorValues(const RingColorParameter& param, RingColorValues& values, Manager* manager, InstanceGlobal* instanceGlobal)
{
	switch( param.type )
	{
		case RingColorParameter::Fixed:
			values.original = param.fixed;
			values.fixed._color = values.original;
			break;
		case RingColorParameter::Random:
			values.original = param.random.getValue(*instanceGlobal);
			values.random._color = values.original;
			break;
		case RingColorParameter::Easing:
			values.easing.start = param.easing.getStartValue(*instanceGlobal);
			values.easing.end = param.easing.getEndValue(*instanceGlobal);
			values.original = values.easing.start;
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
	if (param.type == RingColorParameter::Fixed)
	{
		values.original = values.fixed._color;
	}
	else if (param.type == RingColorParameter::Random)
	{
		values.original = values.random._color;
	}
	else if( param.type == RingColorParameter::Easing )
	{
		param.easing.setValueToArg(
			values.original, 
			values.easing.start,
			values.easing.end,
			instance.m_LivingTime / instance.m_LivedTime );
	}

	float fadeAlpha = GetFadeAlpha(instance);
	if (fadeAlpha != 1.0f)
	{
		values.original.A = (uint8_t)(values.original.A * fadeAlpha);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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

	auto ef = (EffectImplemented*) m_effect;

	memcpy(&RenderingOrder, pos, sizeof(int));
	pos += sizeof(int);

	if (m_effect->GetVersion() >= 3)
	{
		AlphaBlend = RendererCommon.AlphaBlend;
	}
	else
	{
		memcpy(&AlphaBlend, pos, sizeof(int));
		pos += sizeof(int);
		RendererCommon.AlphaBlend = AlphaBlend;
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
		else
		{
			SpritePosition.fixed.ll.x = -0.5f;
			SpritePosition.fixed.ll.y = -0.5f;
			SpritePosition.fixed.lr.x = 0.5f;
			SpritePosition.fixed.lr.y = -0.5f;
			SpritePosition.fixed.ul.x = -0.5f;
			SpritePosition.fixed.ul.y = 0.5f;
			SpritePosition.fixed.ur.x = 0.5f;
			SpritePosition.fixed.ur.y = 0.5f;
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
		SpriteTexture = RendererCommon.ColorTextureIndex;
	}
	else
	{
		memcpy(&SpriteTexture, pos, sizeof(int));
		pos += sizeof(int);
		RendererCommon.ColorTextureIndex = SpriteTexture;
	}

	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
	}

	/* 位置拡大処理 */
	if (ef->IsDyanamicMagnificationValid())
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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		nodeParameter.ZSort = DepthValues.ZSort;

		renderer->BeginRendering( nodeParameter, count, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
{
	const InstanceValues& instValues = instance.rendererValues.sprite;
	SpriteRenderer* renderer = manager->GetSpriteRenderer();
	if( renderer != NULL )
	{
		SpriteRenderer::NodeParameter nodeParameter;
		nodeParameter.AlphaBlend = AlphaBlend;
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		nodeParameter.ZSort = DepthValues.ZSort;

		SpriteRenderer::InstanceParameter instanceParameter;
		instanceParameter.AllColor = instValues._color;

		instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		// Inherit Color
		Color _color;
		if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
		{
			_color = Color::Mul(instValues._originalColor, instance.ColorParent);
		}
		else
		{
			_color = instValues._originalColor;
		}

		Color color_ll = _color;
		Color color_lr = _color;
		Color color_ul = _color;
		Color color_ur = _color;

		if( SpriteColor.type == SpriteColorParameter::Default )
		{
		}
		else if( SpriteColor.type == SpriteColorParameter::Fixed )
		{
			color_ll = Color::Mul( color_ll, SpriteColor.fixed.ll );
			color_lr = Color::Mul( color_lr, SpriteColor.fixed.lr );
			color_ul = Color::Mul( color_ul, SpriteColor.fixed.ul );
			color_ur = Color::Mul( color_ur, SpriteColor.fixed.ur );
		}

		instanceParameter.Colors[0] = color_ll;
		instanceParameter.Colors[1] = color_lr;
		instanceParameter.Colors[2] = color_ul;
		instanceParameter.Colors[3] = color_ur;
		
		// Apply global Color
		if (instance.m_pContainer->GetRootInstance()->IsGlobalColorSet)
		{
			instanceParameter.Colors[0] = Color::Mul(instanceParameter.Colors[0], instance.m_pContainer->GetRootInstance()->GlobalColor);
			instanceParameter.Colors[1] = Color::Mul(instanceParameter.Colors[1], instance.m_pContainer->GetRootInstance()->GlobalColor);
			instanceParameter.Colors[2] = Color::Mul(instanceParameter.Colors[2], instance.m_pContainer->GetRootInstance()->GlobalColor);
			instanceParameter.Colors[3] = Color::Mul(instanceParameter.Colors[3], instance.m_pContainer->GetRootInstance()->GlobalColor);
		}

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
		nodeParameter.TextureFilter = RendererCommon.FilterType;
		nodeParameter.TextureWrap = RendererCommon.WrapType;
		nodeParameter.ZTest = RendererCommon.ZTest;
		nodeParameter.ZWrite = RendererCommon.ZWrite;
		nodeParameter.Billboard = Billboard;
		nodeParameter.ColorTextureIndex = SpriteTexture;
		nodeParameter.EffectPointer = GetEffect();
		nodeParameter.IsRightHand = manager->GetCoordinateSystem() ==
			CoordinateSystem::RH;

		nodeParameter.Distortion = RendererCommon.Distortion;
		nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		nodeParameter.DepthOffset = DepthValues.DepthOffset;
		nodeParameter.IsDepthOffsetScaledWithCamera = DepthValues.IsDepthOffsetScaledWithCamera;
		nodeParameter.IsDepthOffsetScaledWithParticleScale = DepthValues.IsDepthOffsetScaledWithParticleScale;

		nodeParameter.ZSort = DepthValues.ZSort;

		renderer->EndRendering( nodeParameter, m_userData );
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.sprite;
	auto instanceGlobal = instance.m_pContainer->GetRootInstance();

	if( SpriteAllColor.type == StandardColorParameter::Fixed )
	{
		instValues.allColorValues.fixed._color = SpriteAllColor.fixed.all;
		instValues._originalColor = instValues.allColorValues.fixed._color;
	}
	else if( SpriteAllColor.type == StandardColorParameter::Random )
	{
		instValues.allColorValues.random._color = SpriteAllColor.random.all.getValue(*instanceGlobal);
		instValues._originalColor = instValues.allColorValues.random._color;
	}
	else if( SpriteAllColor.type == StandardColorParameter::Easing )
	{
		instValues.allColorValues.easing.start = SpriteAllColor.easing.all.getStartValue(*instanceGlobal);
		instValues.allColorValues.easing.end = SpriteAllColor.easing.all.getEndValue(*instanceGlobal);

		float t = instance.m_LivingTime / instance.m_LivedTime;

		SpriteAllColor.easing.all.setValueToArg(
			instValues._originalColor,
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( SpriteAllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues.allColorValues.fcurve_rgba.offset[0] = SpriteAllColor.fcurve_rgba.FCurve->R.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[1] = SpriteAllColor.fcurve_rgba.FCurve->G.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[2] = SpriteAllColor.fcurve_rgba.FCurve->B.GetOffset(*instanceGlobal);
		instValues.allColorValues.fcurve_rgba.offset[3] = SpriteAllColor.fcurve_rgba.FCurve->A.GetOffset(*instanceGlobal);
		
		instValues._originalColor.R = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + SpriteAllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.G = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + SpriteAllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.B = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + SpriteAllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.A = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + SpriteAllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._originalColor, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._originalColor;
	}
	
	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeSprite::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	InstanceValues& instValues = instance.rendererValues.sprite;

	if (SpriteAllColor.type == StandardColorParameter::Fixed)
	{
		instValues._originalColor = instValues.allColorValues.fixed._color;
	}
	else if (SpriteAllColor.type == StandardColorParameter::Random)
	{
		instValues._originalColor = instValues.allColorValues.random._color;
	}
	if( SpriteAllColor.type == StandardColorParameter::Easing )
	{
		float t = instance.m_LivingTime / instance.m_LivedTime;

		SpriteAllColor.easing.all.setValueToArg(
			instValues._originalColor, 
			instValues.allColorValues.easing.start,
			instValues.allColorValues.easing.end,
			t );
	}
	else if( SpriteAllColor.type == StandardColorParameter::FCurve_RGBA )
	{
		instValues._originalColor.R = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[0] + SpriteAllColor.fcurve_rgba.FCurve->R.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.G = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[1] + SpriteAllColor.fcurve_rgba.FCurve->G.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.B = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[2] + SpriteAllColor.fcurve_rgba.FCurve->B.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
		instValues._originalColor.A = (uint8_t)Clamp( (instValues.allColorValues.fcurve_rgba.offset[3] + SpriteAllColor.fcurve_rgba.FCurve->A.GetValue( (int32_t)instance.m_LivingTime )), 255, 0);
	}

	float fadeAlpha = GetFadeAlpha(instance);
	if (fadeAlpha != 1.0f)
	{
		instValues._originalColor.A = (uint8_t)(instValues._originalColor.A * fadeAlpha);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		instValues._color = Color::Mul(instValues._originalColor, instance.ColorParent);
	}
	else
	{
		instValues._color = instValues._originalColor;
	}

	instance.ColorInheritance = instValues._color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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
	memcpy(&type, pos, sizeof(int));
	pos += sizeof(int);
	assert(type == GetType());
	EffekseerPrintDebug("Renderer : Track\n");

	LoadValues(TrackSizeFor, pos);
	LoadValues(TrackSizeMiddle, pos);
	LoadValues(TrackSizeBack, pos);

	if (m_effect->GetVersion() >= 13)
	{
		memcpy(&SplineDivision, pos, sizeof(int32_t));
		pos += sizeof(int32_t);
	}

	TrackColorLeft.load(pos, m_effect->GetVersion());
	TrackColorLeftMiddle.load(pos, m_effect->GetVersion());

	TrackColorCenter.load(pos, m_effect->GetVersion());
	TrackColorCenterMiddle.load(pos, m_effect->GetVersion());

	TrackColorRight.load(pos, m_effect->GetVersion());
	TrackColorRightMiddle.load(pos, m_effect->GetVersion());

	AlphaBlend = RendererCommon.AlphaBlend;
	TrackTexture = RendererCommon.ColorTextureIndex;

	EffekseerPrintDebug("TrackColorLeft : %d\n", TrackColorLeft.type);
	EffekseerPrintDebug("TrackColorLeftMiddle : %d\n", TrackColorLeftMiddle.type);
	EffekseerPrintDebug("TrackColorCenter : %d\n", TrackColorCenter.type);
	EffekseerPrintDebug("TrackColorCenterMiddle : %d\n", TrackColorCenterMiddle.type);
	EffekseerPrintDebug("TrackColorRight : %d\n", TrackColorRight.type);
	EffekseerPrintDebug("TrackColorRightMiddle : %d\n", TrackColorRightMiddle.type);

	// 右手系左手系変換
	if (setting->GetCoordinateSystem() == CoordinateSystem::LH)
	{
	}

	/* 位置拡大処理 */
	if (m_effect->GetVersion() >= 8)
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
	if (renderer != NULL)
	{
		m_nodeParameter.AlphaBlend = AlphaBlend;
		m_nodeParameter.TextureFilter = RendererCommon.FilterType;
		m_nodeParameter.TextureWrap = RendererCommon.WrapType;
		m_nodeParameter.ZTest = RendererCommon.ZTest;
		m_nodeParameter.ZWrite = RendererCommon.ZWrite;
		m_nodeParameter.ColorTextureIndex = TrackTexture;
		m_nodeParameter.EffectPointer = GetEffect();

		m_nodeParameter.Distortion = RendererCommon.Distortion;
		m_nodeParameter.DistortionIntensity = RendererCommon.DistortionIntensity;

		m_nodeParameter.SplineDivision = SplineDivision;

		renderer->BeginRendering(m_nodeParameter, count, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::BeginRenderingGroup(InstanceGroup* group, Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if (renderer != nullptr)
	{
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
		renderer->BeginRenderingGroup(m_nodeParameter, group->GetInstanceCount(), m_userData);
	}
}

void EffectNodeTrack::EndRenderingGroup(InstanceGroup* group, Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if (renderer != NULL)
	{
		renderer->EndRenderingGroup(m_nodeParameter, group->GetInstanceCount(), m_userData);
	}
}

void EffectNodeTrack::Rendering(const Instance& instance, const Instance* next_instance, Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if (renderer != NULL)
	{
		float t = (float)instance.m_LivingTime / (float)instance.m_LivedTime;
		int32_t time = (int32_t)instance.m_LivingTime;
		int32_t livedTime = (int32_t)instance.m_LivedTime;

		SetValues(m_instanceParameter.ColorLeft, instance, m_currentGroupValues.ColorLeft, TrackColorLeft, time, livedTime);
		SetValues(m_instanceParameter.ColorCenter, instance, m_currentGroupValues.ColorCenter, TrackColorCenter, time, livedTime);
		SetValues(m_instanceParameter.ColorRight, instance, m_currentGroupValues.ColorRight, TrackColorRight, time, livedTime);

		SetValues(m_instanceParameter.ColorLeftMiddle, instance, m_currentGroupValues.ColorLeftMiddle, TrackColorLeftMiddle, time, livedTime);
		SetValues(m_instanceParameter.ColorCenterMiddle, instance, m_currentGroupValues.ColorCenterMiddle, TrackColorCenterMiddle, time, livedTime);
		SetValues(m_instanceParameter.ColorRightMiddle, instance, m_currentGroupValues.ColorRightMiddle, TrackColorRightMiddle, time, livedTime);

		SetValues(m_instanceParameter.SizeFor, m_currentGroupValues.SizeFor, TrackSizeFor, t);
		SetValues(m_instanceParameter.SizeMiddle, m_currentGroupValues.SizeMiddle, TrackSizeMiddle, t);
		SetValues(m_instanceParameter.SizeBack, m_currentGroupValues.SizeBack, TrackSizeBack, t);

		m_instanceParameter.SRTMatrix43 = instance.GetGlobalMatrix43();

		renderer->Rendering(m_nodeParameter, m_instanceParameter, m_userData);
		m_instanceParameter.InstanceIndex++;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::EndRendering(Manager* manager)
{
	TrackRenderer* renderer = manager->GetTrackRenderer();
	if (renderer != NULL)
	{
		renderer->EndRendering(m_nodeParameter, m_userData);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeRenderedInstanceGroup(InstanceGroup& instanceGroup, Manager* manager)
{
	InstanceGroupValues& instValues = instanceGroup.rendererValues.track;
	auto instanceGlobal = instanceGroup.GetInstanceGlobal();

	InitializeValues(instValues.ColorLeft, TrackColorLeft, instanceGlobal);
	InitializeValues(instValues.ColorCenter, TrackColorCenter, instanceGlobal);
	InitializeValues(instValues.ColorRight, TrackColorRight, instanceGlobal);

	InitializeValues(instValues.ColorLeftMiddle, TrackColorLeftMiddle, instanceGlobal);
	InitializeValues(instValues.ColorCenterMiddle, TrackColorCenterMiddle, instanceGlobal);
	InitializeValues(instValues.ColorRightMiddle, TrackColorRightMiddle, instanceGlobal);

	InitializeValues(instValues.SizeFor, TrackSizeFor, manager);
	InitializeValues(instValues.SizeBack, TrackSizeBack, manager);
	InitializeValues(instValues.SizeMiddle, TrackSizeMiddle, manager);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeRenderedInstance(Instance& instance, Manager* manager)
{
	// Calculate only center
	int32_t time = (int32_t)instance.m_LivingTime;
	int32_t livedTime = (int32_t)instance.m_LivedTime;

	Color c;
	SetValues(c, instance, m_currentGroupValues.ColorCenterMiddle, TrackColorCenterMiddle, time, livedTime);

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		c = Color::Mul(c, instance.ColorParent);
	}

	instance.ColorInheritance = c;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::UpdateRenderedInstance(Instance& instance, Manager* manager)
{
	// Calculate only center
	int32_t time = (int32_t)instance.m_LivingTime;
	int32_t livedTime = (int32_t)instance.m_LivedTime;

	Color c;
	SetValues(c, instance, m_currentGroupValues.ColorCenterMiddle, TrackColorCenterMiddle, time, livedTime);

	instance.ColorInheritance = c;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeValues(InstanceGroupValues::Color& value, StandardColorParameter& param, InstanceGlobal* instanceGlobal)
{
	if (param.type == StandardColorParameter::Fixed)
	{
		value.color.fixed.color_ = param.fixed.all;
	}
	else if (param.type == StandardColorParameter::Random)
	{
		value.color.random.color_ = param.random.all.getValue(*(instanceGlobal));
	}
	else if (param.type == StandardColorParameter::Easing)
	{
		value.color.easing.start = param.easing.all.getStartValue(*(instanceGlobal));
		value.color.easing.end = param.easing.all.getEndValue(*(instanceGlobal));
	}
	else if (param.type == StandardColorParameter::FCurve_RGBA)
	{
		value.color.fcurve_rgba.offset[0] = param.fcurve_rgba.FCurve->R.GetOffset(*instanceGlobal);
		value.color.fcurve_rgba.offset[1] = param.fcurve_rgba.FCurve->G.GetOffset(*instanceGlobal);
		value.color.fcurve_rgba.offset[2] = param.fcurve_rgba.FCurve->B.GetOffset(*instanceGlobal);
		value.color.fcurve_rgba.offset[3] = param.fcurve_rgba.FCurve->A.GetOffset(*instanceGlobal);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::InitializeValues(InstanceGroupValues::Size& value, TrackSizeParameter& param, Manager* manager)
{
	if (param.type == TrackSizeParameter::Fixed)
	{
		value.size.fixed.size_ = param.fixed.size;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::SetValues(Color& c, const Instance& instance, InstanceGroupValues::Color& value, StandardColorParameter& param, int32_t time, int32_t livedTime)
{
	if (param.type == StandardColorParameter::Fixed)
	{
		c = value.color.fixed.color_;
	}
	else if (param.type == StandardColorParameter::Random)
	{
		c = value.color.random.color_;
	}
	else if (param.type == StandardColorParameter::Easing)
	{
		float t = (float)time / (float)livedTime;
		param.easing.all.setValueToArg(
			c,
			value.color.easing.start,
			value.color.easing.end,
			t);
	}
	else if (param.type == StandardColorParameter::FCurve_RGBA)
	{
		c.R = (uint8_t)Clamp((value.color.fcurve_rgba.offset[0] + param.fcurve_rgba.FCurve->R.GetValue((int32_t)time)), 255, 0);
		c.G = (uint8_t)Clamp((value.color.fcurve_rgba.offset[1] + param.fcurve_rgba.FCurve->G.GetValue((int32_t)time)), 255, 0);
		c.B = (uint8_t)Clamp((value.color.fcurve_rgba.offset[2] + param.fcurve_rgba.FCurve->B.GetValue((int32_t)time)), 255, 0);
		c.A = (uint8_t)Clamp((value.color.fcurve_rgba.offset[3] + param.fcurve_rgba.FCurve->A.GetValue((int32_t)time)), 255, 0);
	}

	if (RendererCommon.ColorBindType == BindType::Always || RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		c = Color::Mul(c, instance.ColorParent);
	}

	float fadeAlpha = GetFadeAlpha(instance);
	if (fadeAlpha != 1.0f)
	{
		c.A = (uint8_t)(c.A * fadeAlpha);
	}

	// Apply global Color
	if (instance.m_pContainer->GetRootInstance()->IsGlobalColorSet)
	{
		c = Color::Mul(c, instance.m_pContainer->GetRootInstance()->GlobalColor);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::SetValues(float& s, InstanceGroupValues::Size& value, TrackSizeParameter& param, float time)
{
	if (param.type == TrackSizeParameter::Fixed)
	{
		s = value.size.fixed.size_;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectNodeTrack::LoadValues(TrackSizeParameter& param, unsigned char*& pos)
{
	memcpy(&param.type, pos, sizeof(int));
	pos += sizeof(int);

	if (param.type == TrackSizeParameter::Fixed)
	{
		memcpy(&param.fixed, pos, sizeof(param.fixed));
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

static std::u16string getFilenameWithoutExt(const char16_t* path)
{
	int start = 0;
	int end = 0;

	for (int i = 0; path[i] != 0; i++)
	{
		if (path[i] == u'/' || path[i] == u'\\')
		{
			start = i;
		}
	}

	for (int i = start; path[i] != 0; i++)
	{
		if (path[i] == u'.')
		{
			end = i;
		}
	}

	std::vector<char16_t> ret;

	for (int i = start; i < end; i++)
	{
		ret.push_back(path[i]);
	}
	ret.push_back(0);

	return std::u16string(ret.data());
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

	effect->SetName(getFilenameWithoutExt(path).c_str());

	return effect;
}

void EffectImplemented::ResetReloadingBackup()
{
	if(reloadingBackup == nullptr) return;

	Setting* loader = GetSetting();

	TextureLoader* textureLoader = loader->GetTextureLoader();
	if (textureLoader != NULL)
	{
		for (auto it: reloadingBackup->images.GetCollection())
		{
			textureLoader->Unload(it.second.value);
		}
	
		for (auto it : reloadingBackup->normalImages.GetCollection())
		{
			textureLoader->Unload(it.second.value);
		}

		for (auto it : reloadingBackup->distortionImages.GetCollection())
		{
			textureLoader->Unload(it.second.value);
		}
	}

	SoundLoader* soundLoader = loader->GetSoundLoader();
	if (soundLoader != NULL)
	{
		for (auto it : reloadingBackup->sounds.GetCollection())
		{
			soundLoader->Unload(it.second.value);
		}
	}

	{
		ModelLoader* modelLoader = loader->GetModelLoader();
		if (modelLoader != NULL)
		{
			for (auto it : reloadingBackup->models.GetCollection())
			{
				modelLoader->Unload(it.second.value);
			}
		}
	}

	reloadingBackup.reset();
}


Effect* EffectImplemented::Create( Manager* pManager, void* pData, int size, float magnification, const EFK_CHAR* materialPath )
{
	if( pData == NULL || size == 0 ) return NULL;

	EffectImplemented* effect = new EffectImplemented( pManager, pData, size );
	if ( !effect->Load( pData, size, magnification, materialPath, ReloadingThreadType::Main) )
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

	effect->SetName(getFilenameWithoutExt(path).c_str());

	return effect;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* EffectImplemented::Create( Setting* setting, void* pData, int size, float magnification, const EFK_CHAR* materialPath )
{
	if( pData == NULL || size == 0 ) return NULL;

	EffectImplemented* effect = new EffectImplemented( setting, pData, size );
	if ( !effect->Load( pData, size, magnification, materialPath, ReloadingThreadType::Main) )
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
	, m_normalImageCount(0)
	, m_normalImagePaths(nullptr)
	, m_normalImages(nullptr)
	, m_distortionImageCount(0)
	, m_distortionImagePaths(nullptr)
	, m_distortionImages(nullptr)
	, m_WaveCount		( 0 )
	, m_WavePaths		( NULL )
	, m_pWaves			( NULL )
	, m_modelCount		( 0 )
	, m_modelPaths		( NULL )
	, m_pModels			( NULL )
	, m_maginification	( 1.0f )
	, m_maginificationExternal	( 1.0f )
	, m_defaultRandomSeed	(-1)
	, m_pRoot			( NULL )

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
	ResetReloadingBackup();
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
bool EffectImplemented::Load( void* pData, int size, float mag, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType)
{
	// if reladingThreadType == ReloadingThreadType::Main, this function was regarded as loading function actually

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
		m_pImages = new TextureData*[ m_ImageCount ];

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
			m_normalImages = new TextureData*[m_normalImageCount];

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
			m_distortionImages = new TextureData*[m_distortionImageCount];

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

	if (m_version >= 13)
	{
		memcpy(&renderingNodesCount, pos, sizeof(int32_t));
		pos += sizeof(int32_t);

		memcpy(&renderingNodesThreshold, pos, sizeof(int32_t));
		pos += sizeof(int32_t);	
	}

	// 拡大率
	if( m_version >= 2 )
	{
		memcpy( &m_maginification, pos, sizeof(float) );
		pos += sizeof(float);
	}

	m_maginification *= mag;
	m_maginificationExternal = mag;

	if (m_version >= 11)
	{
		memcpy(&m_defaultRandomSeed, pos, sizeof(int32_t));
		pos += sizeof(int32_t);
	}
	else
	{
		m_defaultRandomSeed = -1;
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

bool EffectImplemented::IsDyanamicMagnificationValid() const
{
	return GetVersion() >= 8 || GetVersion() < 2;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Manager* EffectImplemented::GetManager() const
{
	return m_pManager;	
}

const char16_t* EffectImplemented::GetName() const
{
	return name_.c_str();
}

void EffectImplemented::SetName(const char16_t* name)
{
	name_ = name;
}

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
TextureData* EffectImplemented::GetColorImage( int n ) const
{
	return m_pImages[ n ];
}

int32_t EffectImplemented::GetColorImageCount() const
{
	return m_ImageCount;
}

TextureData* EffectImplemented::GetNormalImage(int n) const
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

TextureData* EffectImplemented::GetDistortionImage(int n) const
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
bool EffectImplemented::Reload( void* data, int32_t size, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType)
{
	if(m_pManager == NULL ) return false;

	std::array<Manager*, 1> managers;
	managers[0] = m_pManager;

	return Reload( managers.data(), managers.size(), data, size, materialPath, reloadingThreadType);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( const EFK_CHAR* path, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType)
{
	if(m_pManager == NULL ) return false;

	std::array<Manager*, 1> managers;
	managers[0] = m_pManager;

	return Reload(managers.data(), managers.size(), path, materialPath, reloadingThreadType);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( Manager** managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType)
{
	const EFK_CHAR* matPath = materialPath != NULL ? materialPath : m_materialPath.c_str();
	
	int lockCount = 0;

	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)managers[i])->BeginReloadEffect( this, lockCount == 0);
		lockCount++;
	}

	// HACK for scale
	auto originalMag = this->GetMaginification() / this->m_maginificationExternal;
	auto originalMagExt = this->m_maginificationExternal;

	isReloadingOnRenderingThread = true;
	Reset();
	Load( data, size, originalMag * originalMagExt, matPath, reloadingThreadType);

	// HACK for scale
	m_maginification = originalMag * originalMagExt;
	m_maginificationExternal = originalMagExt;

	isReloadingOnRenderingThread = false;

	for( int32_t i = 0; i < managersCount; i++)
	{
		lockCount--;
		((ManagerImplemented*)managers[i])->EndReloadEffect( this, lockCount == 0);
	}

	return false;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool EffectImplemented::Reload( Manager** managers, int32_t managersCount, const EFK_CHAR* path, const EFK_CHAR* materialPath, ReloadingThreadType reloadingThreadType)
{
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

	int lockCount = 0;

	for( int32_t i = 0; i < managersCount; i++)
	{
		((ManagerImplemented*)&(managers[i]))->BeginReloadEffect( this, lockCount == 0);
		lockCount++;
	}

	isReloadingOnRenderingThread = true;
	Reset();
	Load( data, size, m_maginificationExternal, materialPath, reloadingThreadType);
	isReloadingOnRenderingThread = false;

	for( int32_t i = 0; i < managersCount; i++)
	{
		lockCount--;
		((ManagerImplemented*)&(managers[i]))->EndReloadEffect( this, lockCount == 0);
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

	// reloading on render thread
	if (isReloadingOnRenderingThread)
	{
		assert(reloadingBackup != nullptr);

		for (int32_t ind = 0; ind < m_ImageCount; ind++)
		{
			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_ImagePaths[ind]);

			TextureData* value = nullptr;
			if (reloadingBackup->images.Pop(fullPath, value))
			{
				m_pImages[ind] = value;
			}
		}
	
		for (int32_t ind = 0; ind < m_normalImageCount; ind++)
		{
			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_normalImagePaths[ind]);
			
			TextureData* value = nullptr;
			if (reloadingBackup->normalImages.Pop(fullPath, value))
			{
				m_normalImages[ind] = value;
			}
		}
				
		for (int32_t ind = 0; ind < m_distortionImageCount; ind++)
		{
			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_distortionImagePaths[ind]);
			
			TextureData* value = nullptr;
			if (reloadingBackup->distortionImages.Pop(fullPath, value))
			{
				m_distortionImages[ind] = value;
			}
		}
				
		for (int32_t ind = 0; ind < m_WaveCount; ind++)
		{
			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_WavePaths[ind]);

			void* value = nullptr;
			if (reloadingBackup->sounds.Pop(fullPath, value))
			{
				m_pWaves[ind] = value;
			}
		}
		
		
		
		for (int32_t ind = 0; ind < m_modelCount; ind++)
		{
			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_modelPaths[ind]);
			
			void* value = nullptr;
			if (reloadingBackup->models.Pop(fullPath, value))
			{
				m_pModels[ind] = value;
			}
		}
			
		return;
	}

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

void EffectImplemented::UnloadResources(const EFK_CHAR* materialPath)
{
	Setting* loader = GetSetting();

	// reloading on render thread
	if (isReloadingOnRenderingThread)
	{
		if (reloadingBackup == nullptr)
		{
			reloadingBackup = std::unique_ptr<EffectReloadingBackup>(new EffectReloadingBackup());
		}

		const EFK_CHAR* matPath = materialPath != nullptr ? materialPath : m_materialPath.c_str();

		for (int32_t ind = 0; ind < m_ImageCount; ind++)
		{
			if (m_pImages[ind] == nullptr) continue;

			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_ImagePaths[ind]);
			reloadingBackup->images.Push(fullPath, m_pImages[ind]);
		}

		for (int32_t ind = 0; ind < m_normalImageCount; ind++)
		{
			if (m_normalImages[ind] == nullptr) continue;

			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_normalImagePaths[ind]);
			reloadingBackup->normalImages.Push(fullPath, m_normalImages[ind]);
		}

		for (int32_t ind = 0; ind < m_distortionImageCount; ind++)
		{
			if (m_distortionImagePaths[ind] == nullptr) continue;

			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_distortionImagePaths[ind]);
			reloadingBackup->distortionImages.Push(fullPath, m_distortionImages[ind]);
		}

		for (int32_t ind = 0; ind < m_WaveCount; ind++)
		{
			if (m_pWaves[ind] == nullptr) continue;

			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_WavePaths[ind]);
			reloadingBackup->sounds.Push(fullPath, m_pWaves[ind]);
		}

		for (int32_t ind = 0; ind < m_modelCount; ind++)
		{
			if (m_pModels[ind] == nullptr) continue;

			EFK_CHAR fullPath[512];
			PathCombine(fullPath, matPath, m_modelPaths[ind]);
			reloadingBackup->models.Push(fullPath, m_pModels[ind]);
		}

		return;
	}
	else
	{
		ResetReloadingBackup();
	}

	TextureLoader* textureLoader = loader->GetTextureLoader();
	if (textureLoader != NULL)
	{
		for (int32_t ind = 0; ind < m_ImageCount; ind++)
		{
			textureLoader->Unload(m_pImages[ind]);
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
	if (soundLoader != NULL)
	{
		for (int32_t ind = 0; ind < m_WaveCount; ind++)
		{
			soundLoader->Unload(m_pWaves[ind]);
			m_pWaves[ind] = NULL;
		}
	}

	{
		ModelLoader* modelLoader = loader->GetModelLoader();
		if (modelLoader != NULL)
		{
			for (int32_t ind = 0; ind < m_modelCount; ind++)
			{
				modelLoader->Unload(m_pModels[ind]);
				m_pModels[ind] = NULL;
			}
		}
	}
}

void EffectImplemented::UnloadResources()
{
	UnloadResources(nullptr);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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
static int64_t GetTime(void)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

Manager::DrawParameter::DrawParameter()
{
	CameraCullingMask = 1;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Manager* Manager::Create( int instance_max, bool autoFlip )
{
	return new ManagerImplemented( instance_max, autoFlip );
}

Matrix43* ManagerImplemented::DrawSet::GetEnabledGlobalMatrix()
{
	if (IsPreupdated)
	{
		InstanceContainer* pContainer = InstanceContainerPointer;
		if (pContainer == nullptr) return nullptr;

		auto firstGroup = pContainer->GetFirstGroup();
		if (firstGroup == nullptr) return nullptr;

		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();
		if (pInstance == nullptr) return nullptr;

		return &(pInstance->m_GlobalMatrix43);
	}
	else
	{
		return &(GlobalMatrix);
	}
}


void ManagerImplemented::DrawSet::CopyMatrixFromInstanceToRoot()
{
	if (IsPreupdated)
	{
		InstanceContainer* pContainer = InstanceContainerPointer;
		if (pContainer == nullptr) return;

		auto firstGroup = pContainer->GetFirstGroup();
		if (firstGroup == nullptr) return;

		Instance* pInstance = pContainer->GetFirstGroup()->GetFirst();
		if (pInstance == nullptr) return;

		GlobalMatrix = pInstance->m_GlobalMatrix43;
	}
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

			// dispose all instances
			if (drawset.InstanceContainerPointer != nullptr)
			{
				drawset.InstanceContainerPointer->RemoveForcibly(true);
				drawset.InstanceContainerPointer->~InstanceContainer();
				InstanceContainer::operator delete(drawset.InstanceContainerPointer, this);
			}

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
InstanceContainer* ManagerImplemented::CreateInstanceContainer( EffectNode* pEffectNode, InstanceGlobal* pGlobal, bool isRoot, const Matrix43& rootMatrix, Instance* pParent )
{
	InstanceContainer* pContainer = new(this) InstanceContainer(
		this,
		pEffectNode,
		pGlobal,
		pEffectNode->GetChildrenCount() );
	
	for (int i = 0; i < pEffectNode->GetChildrenCount(); i++)
	{
		pContainer->SetChild(i, CreateInstanceContainer(pEffectNode->GetChild(i), pGlobal, false, Matrix43(), nullptr));
	}

	if( isRoot )
	{
		pGlobal->SetRootContainer(pContainer);

		InstanceGroup* group = pContainer->CreateGroup();
		Instance* instance = group->CreateInstance();
		instance->Initialize( NULL, 0, 0, rootMatrix);

		// This group is not generated by an instance, so changed a flag
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
	for (auto& ds : m_DrawSets)
	{
		if( ds.second.GoingToStop )
		{
			InstanceContainer* pContainer = ds.second.InstanceContainerPointer;

			if (pContainer != nullptr)
			{
				pContainer->KillAllInstances(true);
			}

			ds.second.IsRemoving = true;
			if (GetSoundPlayer() != NULL)
			{
				GetSoundPlayer()->StopTag(ds.second.GlobalPointer);
			}
		}

		if(ds.second.GoingToStopRoot )
		{
			InstanceContainer* pContainer = ds.second.InstanceContainerPointer;

			if (pContainer != nullptr)
			{
				pContainer->KillAllInstances(false);
			}
		}
	}
}

ManagerImplemented::ManagerImplemented( int instance_max, bool autoFlip )
	: m_autoFlip	( autoFlip )
	, m_NextHandle	( 0 )
	, m_instance_max	( instance_max )
	, m_setting			( NULL )
	, m_sequenceNumber	( 0 )

	, m_cullingWorld	(NULL)
	, m_culled(false)

	, m_spriteRenderer(NULL)
	, m_ribbonRenderer(NULL)
	, m_ringRenderer(NULL)
	, m_modelRenderer(NULL)
	, m_trackRenderer(NULL)

	, m_soundPlayer(NULL)

	, m_MallocFunc(NULL)
	, m_FreeFunc(NULL)
	, m_randFunc(NULL)
	, m_randMax(0)
{
	m_setting = Setting::Create();

	SetMallocFunc( Malloc );
	SetFreeFunc( Free );
	SetRandFunc( Rand );
	SetRandMax(RAND_MAX);

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
		// always exists before update
		if (!m_DrawSets[handle].IsPreupdated) return true;

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

		auto mat = drawSet.GetEnabledGlobalMatrix();

		if (mat != nullptr)
		{
			return *mat;
		}
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
		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			(*mat_) = mat;
			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
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
		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			location.X = mat_->Value[3][0];
			location.Y = mat_->Value[3][1];
			location.Z = mat_->Value[3][2];
		}
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
		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			mat_->Value[3][0] = x;
			mat_->Value[3][1] = y;
			mat_->Value[3][2] = z;

			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
		}
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
		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			mat_->Value[3][0] += location.X;
			mat_->Value[3][1] += location.Y;
			mat_->Value[3][2] += location.Z;
			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
		}
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

		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			Matrix43 MatRotX, MatRotY, MatRotZ;

			MatRotX.RotationX(x);
			MatRotY.RotationY(y);
			MatRotZ.RotationZ(z);

			Matrix43 r;
			Vector3D s, t;

			mat_->GetSRT(s, r, t);

			r.Indentity();
			Matrix43::Multiple(r, r, MatRotZ);
			Matrix43::Multiple(r, r, MatRotX);
			Matrix43::Multiple(r, r, MatRotY);

			mat_->SetSRT(s, r, t);

			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
		}		
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

		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			Matrix43 r;
			Vector3D s, t;

			mat_->GetSRT(s, r, t);

			r.RotationAxis(axis, angle);

			mat_->SetSRT(s, r, t);

			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
		}
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

		auto mat_ = drawSet.GetEnabledGlobalMatrix();

		if (mat_ != nullptr)
		{
			Matrix43 r;
			Vector3D s, t;

			mat_->GetSRT(s, r, t);

			s.X = x;
			s.Y = y;
			s.Z = z;

			mat_->SetSRT(s, r, t);

			drawSet.CopyMatrixFromInstanceToRoot();
			drawSet.IsParameterChanged = true;
		}
	}
}

void ManagerImplemented::SetAllColor(Handle handle, Color color)
{
	if (m_DrawSets.count(handle) > 0)
	{
		auto& drawSet = m_DrawSets[handle];

		drawSet.GlobalPointer->IsGlobalColorSet = true;
		drawSet.GlobalPointer->GlobalColor = color;
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

		InstanceGlobal* instanceGlobal = drawSet.GlobalPointer;
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

bool ManagerImplemented::GetShown(Handle handle)
{
	if (m_DrawSets.count(handle) > 0)
	{
		return m_DrawSets[handle].IsShown;
	}

	return false;
}

void ManagerImplemented::SetShown( Handle handle, bool shown )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].IsShown = shown;
	}
}

bool ManagerImplemented::GetPaused(Handle handle)
{
	if (m_DrawSets.count(handle) > 0)
	{
		return m_DrawSets[handle].IsPaused;
	}

	return false;
}

void ManagerImplemented::SetPaused( Handle handle, bool paused )
{
	if( m_DrawSets.count( handle ) > 0 )
	{
		m_DrawSets[handle].IsPaused = paused;
	}
}

void ManagerImplemented::SetPausedToAllEffects(bool paused)
{
	std::map<Handle, DrawSet>::iterator it = m_DrawSets.begin();
	while (it != m_DrawSets.end())
	{
		(*it).second.IsPaused = paused;
		++it;
	}
}

int ManagerImplemented::GetLayer(Handle handle)
{
	if (m_DrawSets.count(handle) > 0)
	{
		return m_DrawSets[handle].Layer;
	}
	return 0;
}

void ManagerImplemented::SetLayer(Handle handle, int32_t layer)
{
	if (m_DrawSets.count(handle) > 0)
	{
		m_DrawSets[handle].Layer = layer;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
float ManagerImplemented::GetSpeed(Handle handle) const
{
	auto it = m_DrawSets.find(handle);
	if (it == m_DrawSets.end()) return 0.0f;
	return it->second.Speed;
}


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
		m_renderingMutex.lock();
	}

	// execute preupdate
	for (auto& drawSet : m_DrawSets)
	{
		Preupdate(drawSet.second);
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
		m_renderingMutex.unlock();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Update( float deltaFrame )
{
	BeginUpdate();

	// start to measure time
	int64_t beginTime = ::Effekseer::GetTime();

	for (auto& drawSet : m_DrawSets)
	{
		UpdateHandle(drawSet.second, deltaFrame);
	}

	// end to measure time
	m_updateTime = (int)(Effekseer::GetTime() - beginTime);

	EndUpdate();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::BeginUpdate()
{
	m_renderingMutex.lock();
	m_isLockedWithRenderingMutex = true;

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
	m_renderingMutex.unlock();
	m_isLockedWithRenderingMutex = false;
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
	if (!drawSet.IsPreupdated)
	{
		Preupdate(drawSet);
	}

	float df = drawSet.IsPaused ? 0 : deltaFrame * drawSet.Speed;

	drawSet.InstanceContainerPointer->Update( true, df, drawSet.IsShown );

	if( drawSet.DoUseBaseMatrix )
	{
		drawSet.InstanceContainerPointer->SetBaseMatrix( true, drawSet.BaseMatrix );
	}

	drawSet.GlobalPointer->AddUpdatedFrame( df );
}


void ManagerImplemented::Preupdate(DrawSet& drawSet)
{
	if (drawSet.IsPreupdated) return;

	// Create an instance through a container
	InstanceContainer* pContainer = CreateInstanceContainer(drawSet.ParameterPointer->GetRoot(), drawSet.GlobalPointer, true, drawSet.GlobalMatrix, NULL);

	drawSet.InstanceContainerPointer = pContainer;

	for (int32_t frame = 0; frame < drawSet.StartFrame; frame++)
	{
		UpdateHandle(drawSet, 1);
	}

	drawSet.IsPreupdated = true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::Draw(const Manager::DrawParameter& drawParameter)
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);

	// 開始時間を記録
	int64_t beginTime = ::Effekseer::GetTime();

	if(m_culled)
	{
		for( size_t i = 0; i < m_culledObjects.size(); i++ )
		{
			DrawSet& drawSet = *m_culledObjects[i];

			if( drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					for (auto& c : drawSet.GlobalPointer->RenderedInstanceContainers)
					{
						c->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw( true );
				}
			}
		}
	}
	else
	{
		for( size_t i = 0; i < m_renderingDrawSets.size(); i++ )
		{
			DrawSet& drawSet = m_renderingDrawSets[i];

			if( drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					for (auto& c : drawSet.GlobalPointer->RenderedInstanceContainers)
					{
						c->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw(true);
				}
			}
		}
	}

	// 経過時間を計算
	m_drawTime = (int)(Effekseer::GetTime() - beginTime);
}

void ManagerImplemented::DrawBack(const Manager::DrawParameter& drawParameter)
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);
	
	// 開始時間を記録
	int64_t beginTime = ::Effekseer::GetTime();

	if (m_culled)
	{
		for (size_t i = 0; i < m_culledObjects.size(); i++)
		{
			DrawSet& drawSet = *m_culledObjects[i];

			if (drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				auto e = (EffectImplemented*)drawSet.ParameterPointer;
				for (int32_t j = 0; j < e->renderingNodesThreshold; j++)
				{
					drawSet.GlobalPointer->RenderedInstanceContainers[j]->Draw(false);
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_renderingDrawSets.size(); i++)
		{
			DrawSet& drawSet = m_renderingDrawSets[i];

			if (drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				auto e = (EffectImplemented*)drawSet.ParameterPointer;
				for (int32_t j = 0; j < e->renderingNodesThreshold; j++)
				{
					drawSet.GlobalPointer->RenderedInstanceContainers[j]->Draw(false);
				}
			}
		}
	}

	// 経過時間を計算
	m_drawTime = (int)(Effekseer::GetTime() - beginTime);
}

void ManagerImplemented::DrawFront(const Manager::DrawParameter& drawParameter)
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);

	// 開始時間を記録
	int64_t beginTime = ::Effekseer::GetTime();

	if (m_culled)
	{
		for (size_t i = 0; i < m_culledObjects.size(); i++)
		{
			DrawSet& drawSet = *m_culledObjects[i];

			if (drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					auto e = (EffectImplemented*)drawSet.ParameterPointer;
					for (size_t j = e->renderingNodesThreshold; j < drawSet.GlobalPointer->RenderedInstanceContainers.size(); j++)
					{
						drawSet.GlobalPointer->RenderedInstanceContainers[j]->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw(true);
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_renderingDrawSets.size(); i++)
		{
			DrawSet& drawSet = m_renderingDrawSets[i];

			if (drawSet.IsShown && drawSet.IsAutoDrawing && ((drawParameter.CameraCullingMask & (1 << drawSet.Layer)) != 0))
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					auto e = (EffectImplemented*)drawSet.ParameterPointer;
					for (size_t j = e->renderingNodesThreshold; j < drawSet.GlobalPointer->RenderedInstanceContainers.size(); j++)
					{
						drawSet.GlobalPointer->RenderedInstanceContainers[j]->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw(true);
				}
			}
		}
	}

	// 経過時間を計算
	m_drawTime = (int)(Effekseer::GetTime() - beginTime);
}

Handle ManagerImplemented::Play( Effect* effect, float x, float y, float z )
{
	return Play(effect, Vector3D(x, y, z), 0);
}

Handle ManagerImplemented::Play(Effect* effect, const Vector3D& position, int32_t startFrame)
{
	if (effect == nullptr) return -1;

	auto e = (EffectImplemented*)effect;

	// Create root
	InstanceGlobal* pGlobal = new InstanceGlobal();

	if (e->m_defaultRandomSeed >= 0)
	{
		pGlobal->SetSeed(e->m_defaultRandomSeed);
	}
	else
	{
		pGlobal->SetSeed(GetRandFunc()());
	}

	pGlobal->RenderedInstanceContainers.resize(e->renderingNodesCount);
	for (size_t i = 0; i < pGlobal->RenderedInstanceContainers.size(); i++)
	{
		pGlobal->RenderedInstanceContainers[i] = nullptr;
	}

	// create a dateSet without an instance
	// an instance is created in Preupdate because effects need to show instances without update(0 frame)
	Handle handle = AddDrawSet(effect, nullptr, pGlobal);

	auto& drawSet = m_DrawSets[handle];

	drawSet.GlobalMatrix.Indentity();
	drawSet.GlobalMatrix.Value[3][0] = position.X;
	drawSet.GlobalMatrix.Value[3][1] = position.Y;
	drawSet.GlobalMatrix.Value[3][2] = position.Z;

	drawSet.IsParameterChanged = true;
	drawSet.StartFrame = startFrame;

	return handle;
}

int ManagerImplemented::GetCameraCullingMaskToShowAllEffects()
{
	int mask = 0;

	for (auto& ds : m_DrawSets)
	{
		auto layer = 1 << ds.second.Layer;
		mask |= layer;
	}

	return mask;
}

void ManagerImplemented::DrawHandle( Handle handle )
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);

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
					if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
					{
						for (auto& c : drawSet.GlobalPointer->RenderedInstanceContainers)
						{
							c->Draw(false);
						}
					}
					else
					{
						drawSet.InstanceContainerPointer->Draw(true);
					}
				}
			}
		}
		else
		{
			if( drawSet.IsShown )
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					for (auto& c : drawSet.GlobalPointer->RenderedInstanceContainers)
					{
						c->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw(true);
				}
			}
		}
	}
}

void ManagerImplemented::DrawHandleBack(Handle handle)
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);

	std::map<Handle, DrawSet>::iterator it = m_renderingDrawSetMaps.find(handle);
	if (it != m_renderingDrawSetMaps.end())
	{
		DrawSet& drawSet = it->second;

		if (m_culled)
		{
			if (m_culledObjectSets.find(drawSet.Self) != m_culledObjectSets.end())
			{
				if (drawSet.IsShown)
				{
					auto e = (EffectImplemented*)drawSet.ParameterPointer;
					for (int32_t i = 0; i < e->renderingNodesThreshold; i++)
					{
						drawSet.GlobalPointer->RenderedInstanceContainers[i]->Draw(false);
					}
				}
			}
		}
		else
		{
			if (drawSet.IsShown)
			{
				auto e = (EffectImplemented*)drawSet.ParameterPointer;
				for (int32_t i = 0; i < e->renderingNodesThreshold; i++)
				{
					drawSet.GlobalPointer->RenderedInstanceContainers[i]->Draw(false);
				}
			}
		}
	}
}

void ManagerImplemented::DrawHandleFront(Handle handle)
{
	std::lock_guard<std::mutex> lock(m_renderingMutex);

	std::map<Handle, DrawSet>::iterator it = m_renderingDrawSetMaps.find(handle);
	if (it != m_renderingDrawSetMaps.end())
	{
		DrawSet& drawSet = it->second;

		if (m_culled)
		{
			if (m_culledObjectSets.find(drawSet.Self) != m_culledObjectSets.end())
			{
				if (drawSet.IsShown)
				{
					if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
					{
						auto e = (EffectImplemented*)drawSet.ParameterPointer;
						for (size_t i = e->renderingNodesThreshold; i < drawSet.GlobalPointer->RenderedInstanceContainers.size(); i++)
						{
							drawSet.GlobalPointer->RenderedInstanceContainers[i]->Draw(false);
						}
					}
					else
					{
						drawSet.InstanceContainerPointer->Draw(true);
					}
				}
			}
		}
		else
		{
			if (drawSet.IsShown)
			{
				if (drawSet.GlobalPointer->RenderedInstanceContainers.size() > 0)
				{
					auto e = (EffectImplemented*)drawSet.ParameterPointer;
					for (size_t i = e->renderingNodesThreshold; i < drawSet.GlobalPointer->RenderedInstanceContainers.size(); i++)
					{
						drawSet.GlobalPointer->RenderedInstanceContainers[i]->Draw(false);
					}
				}
				else
				{
					drawSet.InstanceContainerPointer->Draw(true);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ManagerImplemented::BeginReloadEffect( Effect* effect, bool doLockThread)
{
	if (doLockThread)
	{
		m_renderingMutex.lock();
		m_isLockedWithRenderingMutex = true;
	}

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
void ManagerImplemented::EndReloadEffect( Effect* effect, bool doLockThread)
{
	std::map<Handle,DrawSet>::iterator it = m_DrawSets.begin();
	std::map<Handle,DrawSet>::iterator it_end = m_DrawSets.end();

	for(; it != it_end; ++it )
	{
		if( (*it).second.ParameterPointer != effect ) continue;

		auto e = (EffectImplemented*)effect;
		auto pGlobal = (*it).second.GlobalPointer;

		// reallocate
		if (e->m_defaultRandomSeed >= 0)
		{
			pGlobal->SetSeed(e->m_defaultRandomSeed);
		}
		else
		{
			pGlobal->SetSeed(GetRandFunc()());
		}

		pGlobal->RenderedInstanceContainers.resize(e->renderingNodesCount);
		for (size_t i = 0; i < pGlobal->RenderedInstanceContainers.size(); i++)
		{
			pGlobal->RenderedInstanceContainers[i] = nullptr;
		}

		// Create an instance through a container
		(*it).second.InstanceContainerPointer = CreateInstanceContainer( ((EffectImplemented*)effect)->GetRoot(), (*it).second.GlobalPointer, true, (*it).second.GlobalMatrix, NULL );
		
		// skip
		for( float f = 0; f < (*it).second.GlobalPointer->GetUpdatedFrame() - 1; f+= 1.0f )
		{
			(*it).second.InstanceContainerPointer->Update( true, 1.0f, false );
		}

		(*it).second.InstanceContainerPointer->Update( true, 1.0f, (*it).second.IsShown );
	}

	if (doLockThread)
	{
		m_renderingMutex.unlock();
		m_isLockedWithRenderingMutex = false;
	}
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
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

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
void* InstanceContainer::operator new(size_t size, Manager* pManager)
{
	return pManager->GetMallocFunc()((uint32_t)size);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::operator delete(void* p, Manager* pManager)
{
	pManager->GetFreeFunc()(p, sizeof(InstanceContainer));
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer::InstanceContainer(Manager* pManager, EffectNode* pEffectNode, InstanceGlobal* pGlobal, int ChildrenCount)
	: m_pManager(pManager)
	, m_pEffectNode((EffectNodeImplemented*)pEffectNode)
	, m_pGlobal(pGlobal)
	, m_Children(NULL)
	, m_ChildrenCount(ChildrenCount)
	, m_headGroups(NULL)
	, m_tailGroups(NULL)

{
	auto en = (EffectNodeImplemented*)pEffectNode;
	if (en->RenderingPriority >= 0)
	{
		pGlobal->RenderedInstanceContainers[en->RenderingPriority] = this;
	}

	m_Children = (InstanceContainer**)m_pManager->GetMallocFunc()(sizeof(InstanceContainer*) * m_ChildrenCount);
	for (int i = 0; i < m_ChildrenCount; i++)
	{
		m_Children[i] = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer::~InstanceContainer()
{
	RemoveForcibly(false);

	assert(m_headGroups == NULL);
	assert(m_tailGroups == NULL);

	for (int i = 0; i < m_ChildrenCount; i++)
	{
		if (m_Children[i] != NULL)
		{
			m_Children[i]->~InstanceContainer();
			InstanceContainer::operator delete(m_Children[i], m_pManager);
			m_Children[i] = NULL;
		}
	}
	m_pManager->GetFreeFunc()((void*)m_Children, sizeof(InstanceContainer*) * m_ChildrenCount);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
InstanceContainer* InstanceContainer::GetChild(int num)
{
	return m_Children[num];
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::SetChild(int num, InstanceContainer* pContainter)
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

	for (InstanceGroup* group = m_headGroups; group != NULL; )
	{
		if (!group->IsReferencedFromInstance && group->GetInstanceCount() == 0 && group->GetRemovingInstanceCount() == 0)
		{
			InstanceGroup* next = group->NextUsedByContainer;

			delete group;

			if (m_headGroups == group)
			{
				m_headGroups = next;
			}
			group = next;

			if (tailGroup != NULL)
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

	assert(m_tailGroups == NULL || m_tailGroups->NextUsedByContainer == NULL);
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
void InstanceContainer::Update(bool recursive, float deltaFrame, bool shown)
{
	// 更新
	for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
	{
		group->Update(deltaFrame, shown);
	}

	// 破棄
	RemoveInvalidGroups();

	if (recursive)
	{
		for (int i = 0; i < m_ChildrenCount; i++)
		{
			m_Children[i]->Update(recursive, deltaFrame, shown);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::SetBaseMatrix(bool recursive, const Matrix43& mat)
{
	if (m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT)
	{
		for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
		{
			group->SetBaseMatrix(mat);
		}
	}

	if (recursive)
	{
		for (int i = 0; i < m_ChildrenCount; i++)
		{
			m_Children[i]->SetBaseMatrix(recursive, mat);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::RemoveForcibly(bool recursive)
{
	KillAllInstances(false);


	for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
	{
		group->RemoveForcibly();
	}
	RemoveInvalidGroups();

	if (recursive)
	{
		for (int i = 0; i < m_ChildrenCount; i++)
		{
			m_Children[i]->RemoveForcibly(recursive);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::Draw(bool recursive)
{
	if (m_pEffectNode->GetType() != EFFECT_NODE_TYPE_ROOT && m_pEffectNode->GetType() != EFFECT_NODE_TYPE_NONE)
	{
		/* 個数計測 */
		int32_t count = 0;
		{
			for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
			{
				for (auto instance : group->m_instances)
				{
					if (instance->m_State == INSTANCE_STATE_ACTIVE)
					{
						count++;
					}
				}
			}
		}

		if (count > 0)
		{
			/* 描画 */
			m_pEffectNode->BeginRendering(count, m_pManager);

			for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
			{
				m_pEffectNode->BeginRenderingGroup(group, m_pManager);

				if (m_pEffectNode->RenderingOrder == RenderingOrder_FirstCreatedInstanceIsFirst)
				{
					auto it = group->m_instances.begin();

					while (it != group->m_instances.end())
					{
						if ((*it)->m_State == INSTANCE_STATE_ACTIVE)
						{
							auto it_temp = it;
							it_temp++;

							if (it_temp != group->m_instances.end())
							{
								(*it)->Draw((*it_temp));
							}
							else
							{
								(*it)->Draw(nullptr);
							}
						}

						it++;
					}
				}
				else
				{
					auto it = group->m_instances.rbegin();

					while (it != group->m_instances.rend())
					{
						if ((*it)->m_State == INSTANCE_STATE_ACTIVE)
						{
							auto it_temp = it;
							it_temp++;

							if (it_temp != group->m_instances.rend())
							{
								(*it)->Draw((*it_temp));
							}
							else
							{
								(*it)->Draw(nullptr);
							}
						}
						it++;
					}
				}

				m_pEffectNode->EndRenderingGroup(group, m_pManager);
			}

			m_pEffectNode->EndRendering(m_pManager);
		}
	}

	if (recursive)
	{
		for (int i = 0; i < m_ChildrenCount; i++)
		{
			m_Children[i]->Draw(recursive);
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void InstanceContainer::KillAllInstances(bool recursive)
{
	for (InstanceGroup* group = m_headGroups; group != NULL; group = group->NextUsedByContainer)
	{
		group->KillAllInstances();
	}

	if (recursive)
	{
		for (int i = 0; i < m_ChildrenCount; i++)
		{
			m_Children[i]->KillAllInstances(recursive);
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
//----------------------------------------------------------------------------------

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
Instance::Instance(Manager* pManager, EffectNode* pEffectNode, InstanceContainer* pContainer)
	: m_pManager(pManager)
	, m_pEffectNode((EffectNodeImplemented*)pEffectNode)
	, m_pContainer(pContainer)
	, m_headGroups(NULL)
	, m_pParent(NULL)
	, m_State(INSTANCE_STATE_ACTIVE)
	, m_LivedTime(0)
	, m_LivingTime(0)
	, uvTimeOffset(0)
	, m_GlobalMatrix43Calculated(false)
	, m_ParentMatrix43Calculated(false)
	, m_flexibleGeneratedChildrenCount(nullptr)
	, m_flexibleNextGenerationTime(nullptr)
	, m_stepTime(false)
	, m_sequenceNumber(0)
{
	m_generatedChildrenCount = m_fixedGeneratedChildrenCount;
	m_nextGenerationTime = m_fixedNextGenerationTime;
	
	ColorInheritance = Color(255, 255, 255, 255);
	ColorParent = Color(255, 255, 255, 255);

	InstanceGroup* group = NULL;

	for( int i = 0; i < m_pEffectNode->GetChildrenCount(); i++ )
	{
		InstanceContainer* childContainer = m_pContainer->GetChild( i );

		if( group != NULL )
		{
			group->NextUsedByInstance = childContainer->CreateGroup();
			group = group->NextUsedByInstance;
		}
		else
		{
			group = childContainer->CreateGroup();
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

	auto parameter = (EffectNodeImplemented*)m_pEffectNode;

	if (m_flexibleGeneratedChildrenCount != nullptr)
	{
		m_pManager->GetFreeFunc()(m_flexibleGeneratedChildrenCount, sizeof(int32_t) * parameter->GetChildrenCount());
	}

	if (m_flexibleNextGenerationTime != nullptr)
	{
		m_pManager->GetFreeFunc()(m_flexibleNextGenerationTime, sizeof(float) * parameter->GetChildrenCount());
	}
}

void Instance::GenerateChildrenInRequired(float currentTime)
{
	auto instanceGlobal = this->m_pContainer->GetRootInstance();

	auto parameter = (EffectNodeImplemented*)m_pEffectNode;

	InstanceGroup* group = m_headGroups;

	for (int32_t i = 0; i < parameter->GetChildrenCount(); i++, group = group->NextUsedByInstance)
	{
		auto node = (EffectNodeImplemented*)parameter->GetChild(i);
		assert(group != NULL);

		while (true)
		{
			// GenerationTimeOffset can be minus value.
			// Minus frame particles is generated simultaniously at frame 0.
			if (node->CommonValues.MaxGeneration > m_generatedChildrenCount[i] &&
				m_nextGenerationTime[i] <= currentTime)
			{
				// Create a particle
				auto newInstance = group->CreateInstance();
				if (newInstance != nullptr)
				{
					Matrix43 rootMatrix;
					rootMatrix.Indentity();

					newInstance->Initialize(this, m_generatedChildrenCount[i], (int32_t)std::max(0.0f, this->m_LivingTime), rootMatrix);
				}

				m_generatedChildrenCount[i]++;
				m_nextGenerationTime[i] += Max(0.0f, node->CommonValues.GenerationTime.getValue(*instanceGlobal));
			}
			else
			{
				break;
			}
		}
	}
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
void Instance::Initialize( Instance* parent, int32_t instanceNumber, int32_t parentTime, const Matrix43& globalMatrix)
{
	assert(this->m_pContainer != nullptr);
	
	// Invalidate matrix
	m_GlobalMatrix43Calculated = false;
	m_ParentMatrix43Calculated = false;

	auto instanceGlobal = this->m_pContainer->GetRootInstance();

	auto parameter = (EffectNodeImplemented*) m_pEffectNode;

	// Extend array
	if (parameter->GetChildrenCount() >= ChildrenMax)
	{
		m_flexibleGeneratedChildrenCount = (int32_t*)(m_pManager->GetMallocFunc()(sizeof(int32_t) * parameter->GetChildrenCount()));
		m_flexibleNextGenerationTime = (float*)(m_pManager->GetMallocFunc()(sizeof(float) * parameter->GetChildrenCount()));

		m_generatedChildrenCount = m_flexibleGeneratedChildrenCount;
		m_nextGenerationTime = m_flexibleNextGenerationTime;
	}

	// 親の設定
	m_pParent = parent;

	// 子の初期化
	for (int32_t i = 0; i < parameter->GetChildrenCount(); i++)
	{
		auto pNode = (EffectNodeImplemented*) parameter->GetChild(i);

		m_generatedChildrenCount[i] = 0;
		m_nextGenerationTime[i] = pNode->CommonValues.GenerationTimeOffset.getValue(*instanceGlobal);
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
		m_GlobalMatrix43 = globalMatrix;
		m_ParentMatrix43.Indentity();

		// 親の初期化
		m_ParentMatrix43 = GetGlobalMatrix43();

		// Generate zero frame effect
		GenerateChildrenInRequired(0.0f);
		return;
	}
	
	// 親の行列を計算
	m_pParent->CalculateMatrix( 0 );

	// 状態の初期化
	m_State = INSTANCE_STATE_ACTIVE;

	// 時間周りの初期化
	m_LivingTime = 0.0f;
	m_LivedTime = (float)parameter->CommonValues.life.getValue( *instanceGlobal );


	// SRTの初期化
	m_pParent->GetGlobalMatrix43().GetTranslation( m_GlobalPosition );
	m_GlobalRevisionLocation = Vector3D(0.0f, 0.0f, 0.0f);
	m_GlobalRevisionVelocity = Vector3D(0.0f, 0.0f, 0.0f);
	m_GenerationLocation.Indentity();
	m_GlobalMatrix43 = globalMatrix;
	m_ParentMatrix43.Indentity();

	// 親の初期化
	if( parameter->CommonValues.TranslationBindType == BindType::WhenCreating )
	{
		m_ParentMatrix43.Value[3][0] = m_pParent->m_GlobalMatrix43.Value[3][0];
		m_ParentMatrix43.Value[3][1] = m_pParent->m_GlobalMatrix43.Value[3][1];
		m_ParentMatrix43.Value[3][2] = m_pParent->m_GlobalMatrix43.Value[3][2];
	}

	if( parameter->CommonValues.RotationBindType == BindType::WhenCreating &&
		parameter->CommonValues.ScalingBindType == BindType::WhenCreating )
	{
		for( int m = 0; m < 3; m++ )
		{
			for( int n = 0; n < 3; n++ )
			{
				m_ParentMatrix43.Value[m][n] = m_pParent->m_GlobalMatrix43.Value[m][n];
			}
		}
	}
	else if ( parameter->CommonValues.RotationBindType == BindType::WhenCreating )
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
	else if ( parameter->CommonValues.ScalingBindType == BindType::WhenCreating )
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
	
	// Initialize parent color
	if (parameter->RendererCommon.ColorBindType == BindType::Always)
	{
		ColorParent = m_pParent->ColorInheritance;
	}
	else if (parameter->RendererCommon.ColorBindType == BindType::WhenCreating)
	{
		ColorParent = m_pParent->ColorInheritance;
	}

	/* 位置 */
	if( m_pEffectNode->TranslationType == ParameterTranslationType_Fixed )
	{
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_PVA )
	{
		translation_values.random.location = m_pEffectNode->TranslationPVA.location.getValue( *this->m_pContainer->GetRootInstance() );
		translation_values.random.velocity = m_pEffectNode->TranslationPVA.velocity.getValue(*this->m_pContainer->GetRootInstance());
		translation_values.random.acceleration = m_pEffectNode->TranslationPVA.acceleration.getValue(*this->m_pContainer->GetRootInstance());
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_Easing )
	{
		translation_values.easing.start = m_pEffectNode->TranslationEasing.start.getValue(*this->m_pContainer->GetRootInstance());
		translation_values.easing.end = m_pEffectNode->TranslationEasing.end.getValue(*this->m_pContainer->GetRootInstance());
	}
	else if( m_pEffectNode->TranslationType == ParameterTranslationType_FCurve )
	{
		assert( m_pEffectNode->TranslationFCurve != NULL );

		translation_values.fcruve.offset.x = m_pEffectNode->TranslationFCurve->X.GetOffset( *instanceGlobal );
		translation_values.fcruve.offset.y = m_pEffectNode->TranslationFCurve->Y.GetOffset( *instanceGlobal );
		translation_values.fcruve.offset.z = m_pEffectNode->TranslationFCurve->Z.GetOffset( *instanceGlobal );
	}
	
	/* 回転 */
	if( m_pEffectNode->RotationType == ParameterRotationType_Fixed )
	{
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_PVA )
	{
		rotation_values.random.rotation = m_pEffectNode->RotationPVA.rotation.getValue(*instanceGlobal);
		rotation_values.random.velocity = m_pEffectNode->RotationPVA.velocity.getValue(*instanceGlobal);
		rotation_values.random.acceleration = m_pEffectNode->RotationPVA.acceleration.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_Easing )
	{
		rotation_values.easing.start = m_pEffectNode->RotationEasing.start.getValue(*instanceGlobal);
		rotation_values.easing.end = m_pEffectNode->RotationEasing.end.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_AxisPVA )
	{
		rotation_values.axis.random.rotation = m_pEffectNode->RotationAxisPVA.rotation.getValue(*instanceGlobal);
		rotation_values.axis.random.velocity = m_pEffectNode->RotationAxisPVA.velocity.getValue(*instanceGlobal);
		rotation_values.axis.random.acceleration = m_pEffectNode->RotationAxisPVA.acceleration.getValue(*instanceGlobal);
		rotation_values.axis.rotation = rotation_values.axis.random.rotation;
		rotation_values.axis.axis = m_pEffectNode->RotationAxisPVA.axis.getValue(*instanceGlobal);
		rotation_values.axis.axis.normalize();
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_AxisEasing )
	{
		rotation_values.axis.easing.start = m_pEffectNode->RotationAxisEasing.easing.start.getValue(*instanceGlobal);
		rotation_values.axis.easing.end = m_pEffectNode->RotationAxisEasing.easing.end.getValue(*instanceGlobal);
		rotation_values.axis.rotation = rotation_values.axis.easing.start;
		rotation_values.axis.axis = m_pEffectNode->RotationAxisEasing.axis.getValue(*instanceGlobal);
		rotation_values.axis.axis.normalize();
	}
	else if( m_pEffectNode->RotationType == ParameterRotationType_FCurve )
	{
		assert( m_pEffectNode->RotationFCurve != NULL );

		rotation_values.fcruve.offset.x = m_pEffectNode->RotationFCurve->X.GetOffset( *instanceGlobal );
		rotation_values.fcruve.offset.y = m_pEffectNode->RotationFCurve->Y.GetOffset( *instanceGlobal );
		rotation_values.fcruve.offset.z = m_pEffectNode->RotationFCurve->Z.GetOffset( *instanceGlobal );
	}

	/* 拡大縮小 */
	if( m_pEffectNode->ScalingType == ParameterScalingType_Fixed )
	{
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_PVA )
	{
		scaling_values.random.scale = m_pEffectNode->ScalingPVA.Position.getValue(*instanceGlobal);
		scaling_values.random.velocity = m_pEffectNode->ScalingPVA.Velocity.getValue(*instanceGlobal);
		scaling_values.random.acceleration = m_pEffectNode->ScalingPVA.Acceleration.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_Easing )
	{
		scaling_values.easing.start = m_pEffectNode->ScalingEasing.start.getValue(*instanceGlobal);
		scaling_values.easing.end = m_pEffectNode->ScalingEasing.end.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_SinglePVA )
	{
		scaling_values.single_random.scale = m_pEffectNode->ScalingSinglePVA.Position.getValue(*instanceGlobal);
		scaling_values.single_random.velocity = m_pEffectNode->ScalingSinglePVA.Velocity.getValue(*instanceGlobal);
		scaling_values.single_random.acceleration = m_pEffectNode->ScalingSinglePVA.Acceleration.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_SingleEasing )
	{
		scaling_values.single_easing.start = m_pEffectNode->ScalingSingleEasing.start.getValue(*instanceGlobal);
		scaling_values.single_easing.end = m_pEffectNode->ScalingSingleEasing.end.getValue(*instanceGlobal);
	}
	else if( m_pEffectNode->ScalingType == ParameterScalingType_FCurve )
	{
		assert( m_pEffectNode->ScalingFCurve != NULL );

		scaling_values.fcruve.offset.x = m_pEffectNode->ScalingFCurve->X.GetOffset( *instanceGlobal );
		scaling_values.fcruve.offset.y = m_pEffectNode->ScalingFCurve->Y.GetOffset( *instanceGlobal );
		scaling_values.fcruve.offset.z = m_pEffectNode->ScalingFCurve->Z.GetOffset( *instanceGlobal );
	}

	// Spawning Method
	if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_POINT )
	{
		vector3d p = m_pEffectNode->GenerationLocation.point.location.getValue(*instanceGlobal);
		m_GenerationLocation.Translation( p.x, p.y, p.z );
	}
	else if (m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_LINE)
	{
		vector3d s = m_pEffectNode->GenerationLocation.line.position_start.getValue(*instanceGlobal);
		vector3d e = m_pEffectNode->GenerationLocation.line.position_end.getValue(*instanceGlobal);
		auto noize = m_pEffectNode->GenerationLocation.line.position_noize.getValue(*instanceGlobal);
		auto division = Max(1, m_pEffectNode->GenerationLocation.line.division);

		Vector3D dir;
		(e - s).setValueToArg(dir);

		if (Vector3D::LengthSq(dir) < 0.001)
		{
			m_GenerationLocation.Translation(0 ,0, 0);
		}
		else
		{
			auto len = Vector3D::Length(dir);
			dir /= len;
		
			int32_t target = 0;
			if (m_pEffectNode->GenerationLocation.line.type == ParameterGenerationLocation::LineType::Order)
			{
				target = instanceNumber % division;
			}
			else if (m_pEffectNode->GenerationLocation.line.type == ParameterGenerationLocation::LineType::Random)
			{
				target = (int32_t)((division) * instanceGlobal->GetRand());
				if (target == division) target -= 1;
			}

			auto d = 0.0f;
			if (division > 1)
			{
				d = (len / (float)(division-1)) * target;
			}

			d += noize;
		
			s.x += dir.X * d;
			s.y += dir.Y * d;
			s.z += dir.Z * d;

			Vector3D xdir;
			Vector3D ydir;
			Vector3D zdir;

			if (fabs(dir.Y) > 0.999f)
			{
				xdir = dir;
				Vector3D::Cross(zdir, xdir, Vector3D(-1, 0, 0));
				Vector3D::Normal(zdir, zdir);
				Vector3D::Cross(ydir, zdir, xdir);
				Vector3D::Normal(ydir, ydir);
			}
			else
			{
				xdir = dir;
				Vector3D::Cross(ydir, Vector3D(0, 0, 1), xdir);
				Vector3D::Normal(ydir, ydir);
				Vector3D::Cross(zdir, xdir, ydir);
				Vector3D::Normal(zdir, zdir);
			}

			if (m_pEffectNode->GenerationLocation.EffectsRotation)
			{
				m_GenerationLocation.Value[0][0] = xdir.X;
				m_GenerationLocation.Value[0][1] = xdir.Y;
				m_GenerationLocation.Value[0][2] = xdir.Z;

				m_GenerationLocation.Value[1][0] = ydir.X;
				m_GenerationLocation.Value[1][1] = ydir.Y;
				m_GenerationLocation.Value[1][2] = ydir.Z;

				m_GenerationLocation.Value[2][0] = zdir.X;
				m_GenerationLocation.Value[2][1] = zdir.Y;
				m_GenerationLocation.Value[2][2] = zdir.Z;
			}
			else
			{
				m_GenerationLocation.Indentity();
			}

			m_GenerationLocation.Value[3][0] = s.x;
			m_GenerationLocation.Value[3][1] = s.y;
			m_GenerationLocation.Value[3][2] = s.z;
		}
	}
	else if( m_pEffectNode->GenerationLocation.type == ParameterGenerationLocation::TYPE_SPHERE )
	{
		Matrix43 mat_x, mat_y;
		mat_x.RotationX( m_pEffectNode->GenerationLocation.sphere.rotation_x.getValue( *instanceGlobal ) );
		mat_y.RotationY( m_pEffectNode->GenerationLocation.sphere.rotation_y.getValue( *instanceGlobal ) );
		float r = m_pEffectNode->GenerationLocation.sphere.radius.getValue(*instanceGlobal);
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
						instanceGlobal, 
						parentTime,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_VERTEX )
				{
					emitter = model->GetEmitterFromVertex( 
						instanceNumber,
						parentTime,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_VERTEX_RANDOM )
				{
					emitter = model->GetEmitterFromVertex( 
						instanceGlobal,
						parentTime,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_FACE )
				{
					emitter = model->GetEmitterFromFace( 
						instanceNumber,
						parentTime,
						m_pManager->GetCoordinateSystem(), 
						((EffectImplemented*)m_pEffectNode->GetEffect())->GetMaginification() );
				}
				else if( m_pEffectNode->GenerationLocation.model.type == ParameterGenerationLocation::MODELTYPE_FACE_RANDOM )
				{
					emitter = model->GetEmitterFromFace( 
						instanceGlobal,
						parentTime,
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
		float radius = m_pEffectNode->GenerationLocation.circle.radius.getValue(*instanceGlobal);
		float start = m_pEffectNode->GenerationLocation.circle.angle_start.getValue(*instanceGlobal);
		float end = m_pEffectNode->GenerationLocation.circle.angle_end.getValue(*instanceGlobal);
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
			target = (int32_t)( (div) * instanceGlobal->GetRand() );
			if (target == div) target -= 1;
		}

		float angle = (end - start) * ((float)target / (float)div) + start;

		angle += m_pEffectNode->GenerationLocation.circle.angle_noize.getValue(*instanceGlobal);

		Matrix43 mat;
		if (m_pEffectNode->GenerationLocation.circle.axisDirection == ParameterGenerationLocation::AxisType::X)
		{
			mat.RotationX(angle);
			m_GenerationLocation.Translation(0, 0, radius);
		}
		if (m_pEffectNode->GenerationLocation.circle.axisDirection == ParameterGenerationLocation::AxisType::Y)
		{
			mat.RotationY(angle);
			m_GenerationLocation.Translation(radius, 0, 0);
		}
		if (m_pEffectNode->GenerationLocation.circle.axisDirection == ParameterGenerationLocation::AxisType::Z)
		{
			mat.RotationZ(angle);
			m_GenerationLocation.Translation(0, radius, 0);
		}

		
		Matrix43::Multiple( m_GenerationLocation, m_GenerationLocation, mat );
	}

	if( m_pEffectNode->SoundType == ParameterSoundType_Use )
	{
		soundValues.delay = (int32_t)m_pEffectNode->Sound.Delay.getValue( *instanceGlobal );
	}

	// UV
	if (m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_ANIMATION)
	{
		uvTimeOffset = (int32_t)m_pEffectNode->RendererCommon.UV.Animation.StartFrame.getValue(*instanceGlobal);
		uvTimeOffset *= m_pEffectNode->RendererCommon.UV.Animation.FrameLength;
	}
	
	if (m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_SCROLL)
	{
		auto xy = m_pEffectNode->RendererCommon.UV.Scroll.Position.getValue(*instanceGlobal);
		auto zw = m_pEffectNode->RendererCommon.UV.Scroll.Size.getValue(*instanceGlobal);

		uvAreaOffset.X = xy.x;
		uvAreaOffset.Y = xy.y;
		uvAreaOffset.Width = zw.x;
		uvAreaOffset.Height = zw.y;

		m_pEffectNode->RendererCommon.UV.Scroll.Speed.getValue(*instanceGlobal).setValueToArg(uvScrollSpeed);
	}

	if (m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_FCURVE)
	{
		uvAreaOffset.X = m_pEffectNode->RendererCommon.UV.FCurve.Position->X.GetOffset(*instanceGlobal);
		uvAreaOffset.Y = m_pEffectNode->RendererCommon.UV.FCurve.Position->Y.GetOffset(*instanceGlobal);
		uvAreaOffset.Width = m_pEffectNode->RendererCommon.UV.FCurve.Size->X.GetOffset(*instanceGlobal);
		uvAreaOffset.Height = m_pEffectNode->RendererCommon.UV.FCurve.Size->Y.GetOffset(*instanceGlobal);
	}

	m_pEffectNode->InitializeRenderedInstance(*this, m_pManager);

	// Generate zero frame effect
	GenerateChildrenInRequired(0.0f);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Update( float deltaFrame, bool shown )
{
	assert(this->m_pContainer != nullptr);
	
	// Invalidate matrix
	m_GlobalMatrix43Calculated = false;
	m_ParentMatrix43Calculated = false;

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

	if(shown)
	{
		CalculateMatrix( deltaFrame );
	}
	else if( m_pEffectNode->LocationAbs.type != LocationAbsType::None )
	{
		// If attraction forces are not default, updating is needed in each frame.
		CalculateMatrix( deltaFrame );
	}

	// Get parent color.
	if (m_pParent != NULL)
	{
		if (m_pEffectNode->RendererCommon.ColorBindType == BindType::Always)
		{
			ColorParent = m_pParent->ColorInheritance;
		}
	}

	/* 親の削除処理 */
	if (m_pParent != NULL && m_pParent->GetState() != INSTANCE_STATE_ACTIVE)
	{
		CalculateParentMatrix( deltaFrame );
		m_pParent = nullptr;
	}

	/* 時間の進行 */
	if(  m_stepTime )
	{
		m_LivingTime += deltaFrame;
	}

	// Create child particles
	if( m_stepTime && (originalTime <= m_LivedTime || !m_pEffectNode->CommonValues.RemoveWhenLifeIsExtinct) )
	{
		GenerateChildrenInRequired(originalTime + deltaFrame);


		/*
		InstanceGroup* group = m_headGroups;

		for (int i = 0; i < m_pEffectNode->GetChildrenCount(); i++, group = group->NextUsedByInstance)
		{
			auto pNode = (EffectNodeImplemented*) m_pEffectNode->GetChild(i);
			auto pContainer = m_pContainer->GetChild( i );
			assert( group != NULL );

			// Create a particle
			while (true)
			{
				if (pNode->CommonValues.MaxGeneration > m_generatedChildrenCount[i] &&
					originalTime + deltaFrame >= m_nextGenerationTime[i])
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
					m_nextGenerationTime[i] += Max(0.0f, pNode->CommonValues.GenerationTime.getValue(*instanceGlobal));
				}
				else
				{
					break;
				}
			}
		}
		*/
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

			for (int i = 0; i < m_pEffectNode->GetChildrenCount(); i++, group = group->NextUsedByInstance)
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
		if( m_pEffectNode->GetChildrenCount() > 0)
		{
			// Get parent color.
			if (m_pParent != NULL)
			{
				if (m_pEffectNode->RendererCommon.ColorBindType == BindType::Always)
				{
					ColorParent = m_pParent->ColorInheritance;
				}
			}
		}

		// Delete this particle with myself.
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
	// 計算済なら終了
	if( m_GlobalMatrix43Calculated ) return;
	
	//if( m_sequenceNumber == ((ManagerImplemented*)m_pManager)->GetSequenceNumber() ) return;
	m_sequenceNumber = ((ManagerImplemented*)m_pManager)->GetSequenceNumber();

	assert( m_pEffectNode != NULL );
	assert( m_pContainer != NULL );

	// 親の処理
	if( m_pParent != NULL )
	{
		CalculateParentMatrix( deltaFrame );
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

		if( m_pEffectNode->LocationAbs.type != LocationAbsType::None )
		{
			ModifyMatrixFromLocationAbs( deltaFrame );
		}
	}

	m_GlobalMatrix43Calculated = true;
}

void Instance::CalculateParentMatrix( float deltaFrame )
{
	// 計算済なら終了
	if( m_ParentMatrix43Calculated ) return;

	// 親の行列を計算
	m_pParent->CalculateMatrix( deltaFrame );

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
		
		if( (lType == BindType::Always && rType == BindType::Always && sType == BindType::Always) )
		{
			m_ParentMatrix43 = m_pParent->GetGlobalMatrix43();
		}
		else if ( lType == BindType::NotBind_Root && rType == BindType::NotBind_Root && sType == BindType::NotBind_Root )
		{
			m_ParentMatrix43 = rootInstance->GetGlobalMatrix43();
		}
		else
		{
			Vector3D s, t;
			Matrix43 r;

			m_ParentMatrix43.GetSRT( s, r, t );
			
			if( lType == BindType::Always )
			{
				m_pParent->GetGlobalMatrix43().GetTranslation( t );
			}
			else if( lType == BindType::NotBind_Root && rootInstance != NULL )
			{
				rootInstance->GetGlobalMatrix43().GetTranslation( t );
			}
			
			if( rType == BindType::Always )
			{
				m_pParent->GetGlobalMatrix43().GetRotation( r );
			}
			else if( rType == BindType::NotBind_Root && rootInstance != NULL )
			{
				rootInstance->GetGlobalMatrix43().GetRotation( r );
			}
			

			if( sType == BindType::Always )
			{
				m_pParent->GetGlobalMatrix43().GetScale( s );
			}
			else if( sType == BindType::NotBind_Root && rootInstance != NULL )
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

	m_ParentMatrix43Calculated = true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::ModifyMatrixFromLocationAbs( float deltaFrame )
{
	InstanceGlobal* instanceGlobal = m_pContainer->GetRootInstance();

	// Update attraction forces
	if( m_pEffectNode->LocationAbs.type == LocationAbsType::None )
	{	
	}
	else if( m_pEffectNode->LocationAbs.type == LocationAbsType::Gravity )
	{
		m_GlobalRevisionLocation.X = m_pEffectNode->LocationAbs.gravity.x *
			m_LivingTime * m_LivingTime * 0.5f;
		m_GlobalRevisionLocation.Y = m_pEffectNode->LocationAbs.gravity.y *
			m_LivingTime * m_LivingTime * 0.5f;
		m_GlobalRevisionLocation.Z = m_pEffectNode->LocationAbs.gravity.z *
			m_LivingTime * m_LivingTime * 0.5f;
	}
	else if( m_pEffectNode->LocationAbs.type == LocationAbsType::AttractiveForce )
	{
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

			if (deltaFrame > 0)
			{
				float eps = 0.0001f;
				m_GlobalRevisionVelocity += targetDirection * force * deltaFrame;
				float currentVelocity = Vector3D::Length(m_GlobalRevisionVelocity) + eps;
				Vector3D currentDirection = m_GlobalRevisionVelocity / currentVelocity;

				m_GlobalRevisionVelocity = (targetDirection * control + currentDirection * (1.0f - control)) * currentVelocity;
				m_GlobalRevisionLocation += m_GlobalRevisionVelocity * deltaFrame;
			}
		}
	}

	Matrix43 MatTraGlobal;
	MatTraGlobal.Translation( m_GlobalRevisionLocation.X, m_GlobalRevisionLocation.Y, m_GlobalRevisionLocation.Z );
	Matrix43::Multiple( m_GlobalMatrix43, m_GlobalMatrix43, MatTraGlobal );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Instance::Draw(Instance* next)
{
	assert( m_pEffectNode != NULL );

	if( !m_pEffectNode->IsRendered ) return;

	if( m_sequenceNumber != ((ManagerImplemented*)m_pManager)->GetSequenceNumber() )
	{
		CalculateMatrix( 0 );
	}

	m_pEffectNode->Rendering(*this, next, m_pManager);
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
	if( m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_DEFAULT )
	{
		return RectF( 0.0f, 0.0f, 1.0f, 1.0f );
	}
	else if( m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_FIXED )
	{
		return RectF(
			m_pEffectNode->RendererCommon.UV.Fixed.Position.x,
			m_pEffectNode->RendererCommon.UV.Fixed.Position.y,
			m_pEffectNode->RendererCommon.UV.Fixed.Position.w,
			m_pEffectNode->RendererCommon.UV.Fixed.Position.h );
	}
	else if( m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_ANIMATION )
	{
		auto time = m_LivingTime + uvTimeOffset;

		int32_t frameNum = (int32_t)(time / m_pEffectNode->RendererCommon.UV.Animation.FrameLength);
		int32_t frameCount = m_pEffectNode->RendererCommon.UV.Animation.FrameCountX * m_pEffectNode->RendererCommon.UV.Animation.FrameCountY;

		if( m_pEffectNode->RendererCommon.UV.Animation.LoopType == m_pEffectNode->RendererCommon.UV.Animation.LOOPTYPE_ONCE )
		{
			if( frameNum >= frameCount )
			{
				frameNum = frameCount - 1;
			}
		}
		else if ( m_pEffectNode->RendererCommon.UV.Animation.LoopType == m_pEffectNode->RendererCommon.UV.Animation.LOOPTYPE_LOOP )
		{
			frameNum %= frameCount;
		}
		else if ( m_pEffectNode->RendererCommon.UV.Animation.LoopType == m_pEffectNode->RendererCommon.UV.Animation.LOOPTYPE_REVERSELOOP )
		{
			bool rev = (frameNum / frameCount) % 2 == 1;
			frameNum %= frameCount;
			if( rev )
			{
				frameNum = frameCount - 1 - frameNum;
			}
		}

		int32_t frameX = frameNum % m_pEffectNode->RendererCommon.UV.Animation.FrameCountX;
		int32_t frameY = frameNum / m_pEffectNode->RendererCommon.UV.Animation.FrameCountX;

		return RectF(
			m_pEffectNode->RendererCommon.UV.Animation.Position.x + m_pEffectNode->RendererCommon.UV.Animation.Position.w * frameX,
			m_pEffectNode->RendererCommon.UV.Animation.Position.y + m_pEffectNode->RendererCommon.UV.Animation.Position.h * frameY,
			m_pEffectNode->RendererCommon.UV.Animation.Position.w,
			m_pEffectNode->RendererCommon.UV.Animation.Position.h );
	}
	else if( m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_SCROLL )
	{
		auto time = (int32_t)m_LivingTime + uvTimeOffset;

		return RectF(
			uvAreaOffset.X + uvScrollSpeed.X * time,
			uvAreaOffset.Y + uvScrollSpeed.Y * time,
			uvAreaOffset.Width,
			uvAreaOffset.Height);
	}
	else if (m_pEffectNode->RendererCommon.UVType == ParameterRendererCommon::UV_FCURVE)
	{
		auto time = (int32_t)m_LivingTime + uvTimeOffset;

		return RectF(
			uvAreaOffset.X + m_pEffectNode->RendererCommon.UV.FCurve.Position->X.GetValue(time),
			uvAreaOffset.Y + m_pEffectNode->RendererCommon.UV.FCurve.Position->Y.GetValue(time),
			uvAreaOffset.Width + m_pEffectNode->RendererCommon.UV.FCurve.Size->X.GetValue(time),
			uvAreaOffset.Height + m_pEffectNode->RendererCommon.UV.FCurve.Size->Y.GetValue(time));
	}


	return RectF( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


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

void InstanceGlobal::SetSeed(int32_t seed)
{
	m_seed = seed;
}

float InstanceGlobal::GetRand()
{
	const int a = 1103515245;
	const int c = 12345;
	const int m = 2147483647;
	
	m_seed = (m_seed * a + c) & m;
	auto ret = m_seed % 0x7fff;

	return (float)ret / (float) (0x7fff - 1);
}

float InstanceGlobal::GetRand(float min_, float max_)
{
	return GetRand() * (max_ - min_) + min_;
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
//----------------------------------------------------------------------------------

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
	auto it = m_removingInstances.begin();

	while( it != m_removingInstances.end() )
	{
		auto instance = *it;

		if( instance->m_State == INSTANCE_STATE_ACTIVE )
		{
			assert(0);
		}
		else if( instance->m_State == INSTANCE_STATE_REMOVING )
		{
			// 削除中処理
			instance->m_State = INSTANCE_STATE_REMOVED;
			it++;
		}
		else if( instance->m_State == INSTANCE_STATE_REMOVED )
		{
			it = m_removingInstances.erase( it );

			// 削除処理
			if( instance->m_pEffectNode->GetType() == EFFECT_NODE_TYPE_ROOT )
			{
				delete instance;
			}
			else
			{
				instance->~Instance();
				m_manager->PushInstance( instance );
			}

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

	auto it = m_instances.begin();

	while( it != m_instances.end() )
	{
		auto instance = *it;

		if( instance->m_State == INSTANCE_STATE_ACTIVE )
		{
			// 更新処理
			instance->Update( deltaFrame, shown );

			// 破棄チェック
			if( instance->m_State != INSTANCE_STATE_ACTIVE )
			{
				it = m_instances.erase( it );
				m_removingInstances.push_back( instance );
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
	for (auto instance : m_instances)
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
	while (!m_instances.empty())
	{
		auto instance = m_instances.front();
		m_instances.pop_front();

		if (instance->GetState() == INSTANCE_STATE_ACTIVE)
		{
			instance->Kill();
			m_removingInstances.push_back(instance);
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
//----------------------------------------------------------------------------------
#ifndef	__EFFEKSEER_SOCKET_H__
#define	__EFFEKSEER_SOCKET_H__

#if !( defined(_PSVITA) || defined(_XBOXONE) )

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#if defined(_WIN32) && !defined(_PS4) 
#else

#if !defined(_PS4) 
#endif

#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#if defined(_WIN32) && !defined(_PS4) 

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

#if defined(_WIN32) && !defined(_PS4) 
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

#endif	// #if !( defined(_PSVITA) || defined(_XBOXONE) )

#endif	// __EFFEKSEER_SOCKET_H__

#if !( defined(_PSVITA) || defined(_XBOXONE) )

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#if defined(_WIN32) && !defined(_PS4) 
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
#if defined(_WIN32) && !defined(_PS4) 
	// Initialize  Winsock
	WSADATA m_WsaData;
	::WSAStartup( MAKEWORD(2,0), &m_WsaData );
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Socket::Finalize()
{
#if defined(_WIN32) && !defined(_PS4) 
	// Dispose winsock or decrease a counter
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
#if defined(_WIN32) && !defined(_PS4) 
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
#if defined(_WIN32) && !defined(_PS4) 
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
#if defined(_WIN32) && !defined(_PS4) 
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

#endif	// #if !( defined(_PSVITA) || defined(_XBOXONE) )


#ifndef	__EFFEKSEER_SERVER_IMPLEMENTED_H__
#define	__EFFEKSEER_SERVER_IMPLEMENTED_H__

#if !( defined(_PSVITA) || defined(_SWITCH) || defined(_XBOXONE) )

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
		std::thread		m_threadRecv;
		EfkSocket	m_socket;
		ServerImplemented*		m_server;
		bool		m_active;

		std::vector<uint8_t>	m_recvBuffer;

		std::vector<std::vector<uint8_t> >	m_recvBuffers;
		std::mutex						m_ctrlRecvBuffers;

		static void RecvAsync( void* data );

	public:
		InternalClient( EfkSocket socket_, ServerImplemented* server );
		~InternalClient();
		void ShutDown();
	};

private:
	EfkSocket	m_socket;
	uint16_t	m_port;

	std::thread		m_thread;
	std::mutex		m_ctrlClients;

	bool		m_running;

	std::set<InternalClient*>	m_clients;
	std::set<InternalClient*>	m_removedClients;

	std::map<std::u16string,Effect*>	m_effects;

	std::map<std::u16string,std::vector<uint8_t> >	m_data;

	std::vector<EFK_CHAR>	m_materialPath;

	void AddClient( InternalClient* client );
	void RemoveClient( InternalClient* client );
	static void AcceptAsync( void* data );

public:

	ServerImplemented();
	virtual ~ServerImplemented();

	bool Start( uint16_t port ) override;

	void Stop() override;

	void Register(const EFK_CHAR* key, Effect* effect) override;

	void Unregister(Effect* effect) override;

	void Update(Manager** managers, int32_t managerCount, ReloadingThreadType reloadingThreadType) override;

	void SetMaterialPath( const EFK_CHAR* materialPath ) override;

	void Regist(const EFK_CHAR* key, Effect* effect) override;

	void Unregist(Effect* effect) override;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#endif	// #if !( defined(_PSVITA) || defined(_SWITCH) || defined(_XBOXONE) )

#endif	// __EFFEKSEER_SERVER_IMPLEMENTED_H__

#if !( defined(_PSVITA) || defined(_XBOXONE) )

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------


#if defined(_WIN32) && !defined(_PS4) 
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
			auto recvSize = ::recv( client->m_socket, (char*)(&size), restSize, 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				// Failed
				client->m_server->RemoveClient( client );
				client->ShutDown();
				return;
			}
		}

		restSize = size;
		while(restSize > 0)
		{
			uint8_t buf[256];

			auto recvSize = ::recv( client->m_socket, (char*)(buf), Min(restSize,256), 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				// Failed
				client->m_server->RemoveClient( client );
				client->ShutDown();
				return;
			}

			for( int32_t i = 0; i < recvSize; i++ )
			{
				client->m_recvBuffer.push_back( buf[i] );
			}
		}

		// recieve buffer
		client->m_ctrlRecvBuffers.lock();
		client->m_recvBuffers.push_back(client->m_recvBuffer);
		client->m_ctrlRecvBuffers.unlock();
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
	m_threadRecv = std::thread(
		[this]() 
	{
		RecvAsync(this);
	});
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ServerImplemented::InternalClient::~InternalClient()
{
	m_threadRecv.join();
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
	std::lock_guard<std::mutex> lock(m_ctrlClients);
	m_clients.insert( client );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::RemoveClient( InternalClient* client )
{
	std::lock_guard<std::mutex> lock(m_ctrlClients);
	if( m_clients.count( client ) > 0 )
	{
		m_clients.erase( client );
		m_removedClients.insert( client );
	}
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

		// Accept and add an internal client
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

	// Create a socket
	EfkSocket socket_ = Socket::GenSocket();
	if ( socket_ == InvalidSocket )
	{
		return false;
	}

	memset( &sockAddr, 0, sizeof(SOCKADDR_IN));
	sockAddr.sin_family	= AF_INET;
	sockAddr.sin_port	= htons( port );

	returnCode = ::bind( socket_, (sockaddr*)&sockAddr, sizeof(sockaddr_in) );
	if ( returnCode == SocketError )
	{
		if ( socket_ != InvalidSocket )
		{
			Socket::Close( socket_ );
		}
		return false;
	}

	// Connect
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

	m_thread = std::thread(
		[this]()
	{
		AcceptAsync(this);
	});

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

	m_thread.join();

	// Stop clients
	m_ctrlClients.lock();
	for( std::set<InternalClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it )
	{
		(*it)->ShutDown();
	}
	m_ctrlClients.unlock();
	

	// Wait clients to be removed
	while(true)
	{
		m_ctrlClients.lock();
		int32_t size = (int32_t)m_clients.size();
		m_ctrlClients.unlock();
	
		if( size == 0 ) break;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// Delete clients
	for( std::set<InternalClient*>::iterator it = m_removedClients.begin(); it != m_removedClients.end(); ++it )
	{
		while( (*it)->m_active )
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		delete (*it);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Register( const EFK_CHAR* key, Effect* effect )
{
	if( effect == NULL ) return;

	std::u16string key_( (const char16_t*)key );

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
			m_effects[key_]->Reload( &(m_data[key_][0]), (int32_t)m_data.size(), &(m_materialPath[0]) );
		}
		else
		{
			m_effects[key_]->Reload( &(m_data[key_][0]), (int32_t)m_data.size() );
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ServerImplemented::Unregister( Effect* effect )
{
	if( effect == NULL ) return;

	auto it = m_effects.begin();
	auto it_end = m_effects.end();

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
void ServerImplemented::Update(Manager** managers, int32_t managerCount, ReloadingThreadType reloadingThreadType)
{
	m_ctrlClients.lock();

	for( std::set<InternalClient*>::iterator it = m_removedClients.begin(); it != m_removedClients.end(); ++it )
	{
		while( (*it)->m_active )
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		delete (*it);
	}
	m_removedClients.clear();

	for( std::set<InternalClient*>::iterator it = m_clients.begin(); it != m_clients.end(); ++it )
	{
		(*it)->m_ctrlRecvBuffers.lock();

		for( size_t i = 0; i < (*it)->m_recvBuffers.size(); i++ )
		{
			std::vector<uint8_t>& buf = (*it)->m_recvBuffers[i];

			uint8_t* p = &(buf[0]);
		
			int32_t keylen = 0;
			memcpy( &keylen, p, sizeof(int32_t) );
			p += sizeof(int32_t);

			std::u16string key;
			for( int32_t k = 0; k < keylen; k++ )
			{
				key.push_back( ((char16_t*)p)[0] );
				p += sizeof(char16_t);
			}

			uint8_t* recv_data = p;
			auto datasize = (int32_t)buf.size() - (p-&(buf[0]));
		
			if( m_data.count( key ) > 0 )
			{
				m_data[key].clear();
			}

			for( int32_t d = 0; d < datasize; d++ )
			{
				m_data[key].push_back( recv_data[d] );
			}

			if( m_effects.count( key ) > 0 )
			{
				if (managers != nullptr)
				{
					auto& data_ = m_data[key];

					if (m_materialPath.size() > 1)
					{
						m_effects[key]->Reload(managers, managerCount, data_.data(), (int32_t)data_.size(), &(m_materialPath[0]), reloadingThreadType);
					}
					else
					{
						m_effects[key]->Reload(managers, managerCount, data_.data(), (int32_t)data_.size(), nullptr, reloadingThreadType);
					}
				}
				else
				{
					auto& data_ = m_data[key];

					if (m_materialPath.size() > 1)
					{
						m_effects[key]->Reload(data_.data(), (int32_t)data_.size(), &(m_materialPath[0]), reloadingThreadType);
					}
					else
					{
						m_effects[key]->Reload(data_.data(), (int32_t)data_.size(), nullptr, reloadingThreadType);
					}
				}
				
			}
		}

		(*it)->m_recvBuffers.clear();
		(*it)->m_ctrlRecvBuffers.unlock();

	}
	m_ctrlClients.unlock();
	
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

void ServerImplemented::Regist(const EFK_CHAR* key, Effect* effect)
{
	Register(key, effect);
}

void ServerImplemented::Unregist(Effect* effect)
{
	Unregister(effect);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#endif	// #if !( defined(_PSVITA) || defined(_XBOXONE) )
#ifndef	__EFFEKSEER_CLIENT_IMPLEMENTED_H__
#define	__EFFEKSEER_CLIENT_IMPLEMENTED_H__

#if !( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )

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
	bool isThreadRunning = false;
	std::thread	m_threadRecv;

	EfkSocket	m_socket;
	uint16_t	m_port;
	std::vector<uint8_t>	m_sendBuffer;

	bool		m_running;
	std::mutex	mutexStop;

	bool GetAddr( const char* host, IN_ADDR* addr);

	static void RecvAsync( void* data );
	void StopInternal();
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

#endif	// #if !( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )

#endif	// __EFFEKSEER_CLIENT_IMPLEMENTED_H__

#if !( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )



namespace Effekseer {

void ClientImplemented::RecvAsync( void* data )
{
	auto client = (ClientImplemented*)data;

	while(true)
	{
		int32_t size = 0;
		int32_t restSize = 0;

		restSize = 4;
		while(restSize > 0)
		{
			auto recvSize = ::recv( client->m_socket, (char*)(&size), restSize, 0 );
			restSize -= recvSize;

			if( recvSize == 0 || recvSize == -1 )
			{
				client->StopInternal();
				return;
			}
		}
	}
}

void ClientImplemented::StopInternal()
{
	std::lock_guard<std::mutex> lock(mutexStop);

	if (!m_running) return;
	m_running = false;

	Socket::Shutsown(m_socket);
	Socket::Close(m_socket);

	EffekseerPrintDebug("Client : Stop(Internal)\n");
}

ClientImplemented::ClientImplemented()
	: m_running		( false )
{
	Socket::Initialize();
}

ClientImplemented::~ClientImplemented()
{
	Stop();

	Socket::Finalize();
}

Client* Client::Create()
{
	return new ClientImplemented();
}

/*
HOSTENT* ClientImplemented::GetHostEntry( const char* host )
{
	HOSTENT* hostEntry = nullptr;
	IN_ADDR InAddrHost;

	// check ip adress or DNS
	InAddrHost.s_addr = ::inet_addr( host );
	if ( InAddrHost.s_addr == InaddrNone )
	{
		// DNS
		hostEntry = ::gethostbyname( host );
		if ( hostEntry == nullptr)
		{
			return nullptr;
		}
	}
	else
	{
		// Ip address
		hostEntry = ::gethostbyaddr( (const char*)(&InAddrHost), sizeof(IN_ADDR), AF_INET );
		if ( hostEntry == nullptr)
		{
			return nullptr;
		}
	}

	return hostEntry;
}
*/

bool ClientImplemented::GetAddr(const char* host, IN_ADDR* addr)
{
	HOSTENT* hostEntry = nullptr;

	// check ip adress or DNS
	addr->s_addr = ::inet_addr(host);
	if (addr->s_addr == InaddrNone)
	{
		// DNS
		hostEntry = ::gethostbyname(host);
		if (hostEntry == nullptr)
		{
			return false;
		}

		addr->s_addr = *(unsigned int *)hostEntry->h_addr_list[0];
	}

	return true;
}

bool ClientImplemented::Start( char* host, uint16_t port )
{
	if( m_running ) return false;

	// to stop thread
	Stop();

	SOCKADDR_IN sockAddr;

	// create a socket
	EfkSocket socket_ = Socket::GenSocket();
	if ( socket_ == InvalidSocket )
	{
		return false;
	}

	// get adder
	IN_ADDR addr;
	if (!GetAddr(host, &addr))
	{
		if (socket_ != InvalidSocket) Socket::Close(socket_);
		return false;
	}

	// generate data to connect
	memset( &sockAddr, 0, sizeof(SOCKADDR_IN) );
	sockAddr.sin_family	= AF_INET;
	sockAddr.sin_port	= htons( port );
	sockAddr.sin_addr	= addr;

	// connect
	int32_t ret = ::connect( socket_, (SOCKADDR*)(&sockAddr), sizeof(SOCKADDR_IN) );
	if ( ret == SocketError )
	{
		if ( socket_ != InvalidSocket ) Socket::Close( socket_ );
		return false;
	}

	m_socket = socket_;
	m_port = port;

	m_running = true;

	isThreadRunning = true;
	m_threadRecv = std::thread(
		[this](){
		RecvAsync(this);
	});

	EffekseerPrintDebug("Client : Start\n");

	return true;
}

void ClientImplemented::Stop()
{
	StopInternal();

	if (isThreadRunning)
	{
		m_threadRecv.join();
		isThreadRunning = false;
	}

	EffekseerPrintDebug("Client : Stop\n");
}

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

	int32_t size = (int32_t)m_sendBuffer.size();
	while( size > 0 )
	{
		auto ret = ::send( m_socket, (const char*)(&(m_sendBuffer[m_sendBuffer.size()-size])), size, 0 );
		if( ret == 0 || ret < 0 )
		{
			Stop();
			return false;
		}
		size -= ret;
	}

	return true;
}

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

	Send( &(buf[0]), (int32_t)buf.size() );
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

#endif	// #if !( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
