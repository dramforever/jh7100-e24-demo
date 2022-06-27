{ lib, stdenv, buildPackages, mkShell }:

mkShell {
  CROSS_COMPILE = stdenv.cc.targetPrefix;
  nativeBuildInputs = [ buildPackages.stdenv.cc ];
  hardeningDisable = [ "all" ];
}
