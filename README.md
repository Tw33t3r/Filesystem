# Filesystem
A functional Unix filesystem with a minimal partition manager and disk manager

If this were to be used in a live system, many optimizations would neeed to be made to the fileExists() function, real features would need to be added to the setattributes() function, the usage of something other than a bitvector would be necessary to track files being used, and further debugging may need to be done to ensure no segmentation faults. However, it is still functional without errors in filesystem integrity.

Written initially as a group project by Garrett Tvedt, Ryan Cook, and Moriah Miller.
BitVector code written for the purpose of this project by Jim Ward.

Further edited and optimized for personal portfolio use by Garrett Tvedt.

