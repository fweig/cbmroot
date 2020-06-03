#
##   create STS geometry from patch file
#
# diff -c create_stsgeo_v15b.C create_stsgeo_v15a.C > v15b_to_v15a.patch
#
# cp create_stsgeo_v15b.C create_stsgeo_v15a.C
# patch create_stsgeo_v15a.C v15b_to_v15a.patch
# root -l create_stsgeo_v15a.C
#
SOURCE=create_stsgeo_v15b.C
TARGET=create_stsgeo_v15a.C
PATCH=v15b_to_v15a.patch
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


