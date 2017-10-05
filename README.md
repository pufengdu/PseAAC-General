# PseAAC-General
**This is the current repository of PseAAC-General. Oct. 2nd, 2017**

**The old repositiory here https://sourceforge.net/projects/pseb/files/ is deprecated**

## Installation

Source code in this repositiory is for Ubuntu Linux platform. We have tested these codes on **Ubuntu 14.04.5 LTS**. There is no guarantee that these codes can be compiled and executed on other platforms without modifications.

These codes depend on the lua-devel version 5.3. Please go to http://www.lua.org/download.html. Download a lua environment and configure that on your machine.

After your lua environment is ready, find out the location of your lua.hpp and liblua.a. They usually reside in positions like 
`xxxx/lua-5.3.4/install/include` and `xxxx/lua-5.3.4/install/lib`, where xxxx is usually the directory you unpack your lua source codes.

Suppose the xxxx equals to ~/bin. You should then **EDIT** the following two lines of the Makefile in the directory ***pseb***.

EXT_HEADER=`~/bin/lua-5.3.4/install/include`

EXT_LIB=`~/bin/lua-5.3.4/install/lib`

After saving the Makefile, just return to the PseAAC-General directory and type ***make***. You will get all components of PseAAC-General compiled. You may type ***make install*** to collect all compiled binary executables to the **bin** directory in your PseAAC-General directory.

Please read the manual in the ***doc*** directory before you begin to use this software.

To test your compilation, just find the **pseb** executable and type **./pseb --help**

For ***Microsoft Windows*** platform, just send emails to PufengDu@gmail.com. You will recieve a compiled executable for your Windows environment.

If you got any difficulties, just send emails to PufengDu@gmail.com to complain. We will try out best to fix it asap.

## Citations

If you find PseAAC-General useful in your work, please cite the following two articles.

[1] **Pufeng Du**, Shuwang Gu, Yasen Jiao. PseAAC-General: Fast Building Various Modes of General Form of Chou’s Pseudo-Amino Acid Composition for Large-Scale Protein Datasets. ***International Journal of Molecular Sciences*** 15 (2014), pp.3495-3506 http://dx.doi.org/10.3390/ijms15033495

[2] **Pufeng Du**, Xin Wang, Chao Xu, Yang Gao. PseAAC-Builder: A cross-platform stand-alone program for generating various special Chou's pseudo-amino acid compositions. ***Analytical Biochemistry*** 425 (2012) pp.117-119 https://doi.org/10.1016/j.ab.2012.03.015
