{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }:
    let
      packageNames = with builtins; attrNames (readDir ./packages);
      eachSystem = nixpkgs.lib.genAttrs [ "x86_64-linux" ];
    in {
      devShell = eachSystem (system:
        (import nixpkgs {
          inherit system;
          crossSystem.config = "riscv32-none-elf";
        }).callPackage ./env.nix {});
    };
}
