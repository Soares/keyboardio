let
  srcs = import nix/sources.nix;

  pkgs = import srcs.nixpkgs {};

  keyboardioEnv = pkgs.stdenv.mkDerivation {
    phases = [ "unpackPhase" "installPhase" ];
    name   = "keyboardioEnv";
    src    = pkgs.fetchgit {
      url    = https://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio.git;
      rev    = "577015c";
      sha256 = "1k3qcllqhlz55wmd5ci9kc36431d7cbbimqxn1rvx1rvwqa0zy3d";
    };
    installPhase = ''
      rootDir=$out/hardware/keyboardio
      mkdir -p $rootDir
      cp -r *  $rootDir/
      ln -s ${srcs.Kaleidoscope-CapsLock} $rootDir/avr/libraries/Kaleidoscope-CapsLock
      ln -s ${srcs.Kaleidoscope-ModifierLayers} $rootDir/avr/libraries/Kaleidoscope-ModifierLayers
      ln -s ${srcs.Kaleidoscope-LEDEffect-DigitalRain} $rootDir/avr/libraries/Kaleidoscope-LEDEffect-DigitalRain
    '';
  };
in with pkgs; mkShell {
  name           = "keyboadioShell";
  buildInputs    = [ niv gnumake arduino ];
  ARDUINO_PATH   = "${arduino}/share/arduino";
  SKETCHBOOK_DIR = "${keyboardioEnv}";
}
