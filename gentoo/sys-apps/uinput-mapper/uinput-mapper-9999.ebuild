# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4
PYTHON_DEPEND="2:2.5"

inherit eutils git-2 python

DESCRIPTION="uinput-mapper, create and modify input devices"
HOMEPAGE="http://hetgrotebos.org/wiki/uinput-mapper"
SRC_URI=""

EGIT_REPO_URI="git://github.com/MerlijnWajer/uinput-mapper"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~amd64 ~arm ~x86"
IUSE=""


DEPEND=""
RDEPEND="${DEPEND}"

src_unpack() {
	git-2_src_unpack
}

src_compile() {
	emake
}

src_install() {
	insinto "$(python_get_sitedir)"/uinputmapper
	doins uinputmapper/*.py

	exeinto /usr/bin

	doexe input-read
	doexe input-create
	doexe input-replay
}

