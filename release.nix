{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, buildForAmiga ? false
, amigaosenvPath ? <amigaosenv>
}:

let
  pkgs = import nixpkgs {};
  
  version = builtins.readFile ./version;
  
  jobs = rec {
    tarball =
      { libamivideo ? {outPath = ./.; rev = 1234;}
      , officialRelease ? false
      }:

      with pkgs;

      releaseTools.sourceTarball {
        name = "libamivideo-tarball";
        src = libamivideo;
        inherit version officialRelease;

        buildInputs = [];
      };
      
    build =
      { tarball ? jobs.tarball {} }:
      
      (pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };
        
        releaseTools.nixBuild {
          name = "libamivideo";
          inherit version;
          src = tarball;
        }
      )) //
      (pkgs.lib.optionalAttrs (buildForAmiga)
        (let
          amigaosenv = import amigaosenvPath {
            inherit (pkgs) stdenv uae procps;
            lndir = pkgs.xorg.lndir;
          };
        in
        {
          m68k-amigaos = amigaosenv.mkDerivation {
            name = "libamivideo-${version}";
            src = "${tarball}/tarballs/libamivideo-${version}pre1234.tar.gz";
      
            buildCommand = ''
              tar xfvz $src
              cd libamivideo-${version}pre1234
              CFLAGS='-noixemul -O3' ./configure --prefix=/OUT --disable-shared
              make
              make check
              make install
            '';
          };
        }));
  };
in
jobs
