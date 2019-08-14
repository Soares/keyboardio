with import <nixpkgs> {};

let
  kaleidoscope-capslock = fetchgit {
    url    = https://github.com/malob/Kaleidoscope-CapsLock.git;
    rev    = "bac882d";
    sha256 = "1psyrqv0w443c6f6pnwz9j25s8ghzscf9q9dd849q1lxh0h7zi0s";
  };

  kaleidoscope-modiferlayers = fetchgit {
    url    = https://github.com/malob/Kaleidoscope-ModifierLayers.git;
    rev    = "11e0042";
    sha256 = "0wg9n73pxvk1b1nzxvw3p9kbfdwy24pd0msd7rvnq8js55sxbhbp";
  };

  kaleidoscope-digitalrain = fetchgit {
    url    = https://github.com/tremby/Kaleidoscope-LEDEffect-DigitalRain.git;
    rev    = "6f16e81";
    sha256 = "0amcj216z2qghfdbh7xbqazx21x7q27gshapmgcp726slssx6img";
  };

  keyboardioEnv = stdenv.mkDerivation {
    phases = [ "unpackPhase" "installPhase" ];
    name   = "keyboardioEnv";
    src    = fetchgit {
      url    = https://github.com/keyboardio/Kaleidoscope-Bundle-Keyboardio.git;
      rev    = "b354e5c";
      sha256 = "1j28k0lyxrwd8zy61p3lvjmvq4drx460q55zrnq40y3rrz0dazcl";
    };
    installPhase = ''
      rootDir=$out/hardware/keyboardio
      mkdir -p $rootDir
      cp -r *  $rootDir/
      ln -s ${kaleidoscope-capslock}      $rootDir/avr/libraries/Kaleidoscope-CapsLock
      ln -s ${kaleidoscope-modiferlayers} $rootDir/avr/libraries/Kaleidoscope-ModifierLayers
      ln -s ${kaleidoscope-digitalrain}   $rootDir/avr/libraries/Kaleidoscope-LEDEffect-DigitalRain
    '';
  };
in mkShell {
  name           = "keyboadioEnv";
  buildInputs    = [ gnumake arduino ];
  ARDUINO_PATH   = "${arduino}/share/arduino";
  SKETCHBOOK_DIR = "${keyboardioEnv}";
}
