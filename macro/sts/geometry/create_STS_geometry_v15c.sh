#
##   create STS geometry from patch file
#
# diff -c create_stsgeo_v15b.C create_stsgeo_v15c.C > v15b_to_v15c.patch
#
# cp create_stsgeo_v15b.C create_stsgeo_v15c.C
# patch create_stsgeo_v15c.C v15b_to_v15c.patch
# root -l create_stsgeo_v15c.C
#
SOURCE=create_stsgeo_v15b.C
TARGET=create_stsgeo_v15c.C
PATCH=v15b_to_v15c.patch
#
echo 
echo cp $SOURCE $TARGET
echo patch $TARGET $PATCH
echo root -l $TARGET
echo 
#
cp $SOURCE $TARGET
patch $TARGET $PATCH
root -l $TARGET


