import shutil
import os
from distutils.dir_util import copy_tree

copy_tree("../Effekseer/Dev/Cpp/Effekseer", "./Players/Cocos2d-x_v4/Effekseer")
copy_tree("../Effekseer/Dev/Cpp/EffekseerMaterialCompiler", "./Players/Cocos2d-x_v4/EffekseerMaterialCompiler")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererCommon", "./Players/Cocos2d-x_v4/EffekseerRendererCommon")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererGL", "./Players/Cocos2d-x_v4/EffekseerRendererGL")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererLLGI", "./Players/Cocos2d-x_v4/EffekseerRendererLLGI")
copy_tree("../Effekseer/Dev/Cpp/EffekseerRendererMetal", "./Players/Cocos2d-x_v4/EffekseerRendererMetal")

os.makedirs("./Players/Cocos2d-x_v4/LLGI/", exist_ok=True)
copy_tree("../Effekseer/Dev/Cpp/3rdParty/LLGI/src", "./Players/Cocos2d-x_v4/3rdParty/LLGI/src")
