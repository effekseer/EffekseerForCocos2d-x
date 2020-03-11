import shutil
import os

package_name = 'EffekseerForCocos2d-x/'
if os.path.exists(package_name):
    shutil.rmtree(package_name)
os.makedirs(package_name, exist_ok=False)

shutil.copytree('Players/', package_name + 'Players/')
shutil.copytree('Samples/', package_name +'Samples/')
shutil.copy('Help_En.html', package_name)
shutil.copy('Help_Ja.html', package_name)
shutil.copy('readme.txt', package_name)
shutil.copy('LICENSE', package_name)
