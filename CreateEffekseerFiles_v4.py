import shutil
import os
from distutils.dir_util import copy_tree

root_path = "./Players/Cocos2d-x_v4/"

copy_tree("../Effekseer/Dev/Cpp/Effekseer", "./Players/Cocos2d-x_v4/Effekseer")
copy_tree("../Effekseer/Dev/Cpp/EffekseerMaterialCompiler", "./Players/Cocos2d-x_v4/EffekseerMaterialCompiler")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererCommon", "./Players/Cocos2d-x_v4/EffekseerRendererCommon")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererGL", "./Players/Cocos2d-x_v4/EffekseerRendererGL")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererLLGI", "./Players/Cocos2d-x_v4/EffekseerRendererLLGI")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererMetal", "./Players/Cocos2d-x_v4/EffekseerRendererMetal")

os.makedirs("./Players/Cocos2d-x_v4/3rdParty/LLGI/", exist_ok=True)
shutil.copyfile("../Effekseer/Dev/Cpp/3rdParty/LLGI/CMakeLists.txt", "./Players/Cocos2d-x_v4/3rdParty/LLGI/CMakeLists.txt")
copy_tree("../Effekseer/Dev/Cpp/3rdParty/LLGI/src", "./Players/Cocos2d-x_v4/3rdParty/LLGI/src")
copy_tree("../Effekseer/Dev/Cpp/3rdParty/LLGI/cmake", "./Players/Cocos2d-x_v4/3rdParty/LLGI/cmake")
copy_tree("../Effekseer/Dev/Cpp/3rdParty/stb_effekseer", "./Players/Cocos2d-x_v4/3rdParty/stb_effekseer")

copy_tree("../Effekseer/cmake", "./Players/Cocos2d-x_v4/cmake")


