{ stdenv, fetchFromGitHub, gcc, ftxui, gnumake }:

stdenv.mkDerivation rec {
  pname = "seeker";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner = "Poitew";
    repo = "seeker";
    rev = "v1.0.0";
    sha256 = "sha256";
  };

  buildInputs = [ gcc ftxui gnumake ];

  buildPhase = ''
    make
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp seeker $out/bin/
  '';

  meta = with stdenv.lib; {
    description = "A minimal and lightweight TUI-based file explorer";
    homepage = "https://github.com/Poitew/seeker";
    license = licenses.lgpl3Plus;
    maintainers = with maintainers; [ poitew ];
    platforms = [ platforms.x86_64-linux ];
  };
}
