<p align="center">
<img width="210" height="210" align="left" style="float: left; margin: 0 10px 0 0;" src="https://raw.githubusercontent.com/Plumbum/Plumbum/master/assets/icons/plumbum.png" alt="Plumbum"/>
</br>
<h1>Plumbum - Unleash Your V2Ray</h1> 
使用 Qt 框架的跨平台 V2Ray 客户端。支持 Windows, Linux, macOS
</br>
插件系统支持 SSR / Trojan / Trojan-Go / NaiveProxy
</p>

[![GitHub Releases](https://img.shields.io/github/downloads/Plumbum/Plumbum/latest/total?style=flat-square&logo=github)](https://github.com/Plumbum/Plumbum/releases)
[![GitHub All Releases](https://img.shields.io/github/downloads/Plumbum/Plumbum/total?label=downloads-total&logo=github&style=flat-square)](https://github.com/Plumbum/Plumbum/releases)

## 本项目已不再维护

> Plumbum: *2019-03-28 - 2021-08-17*

自 4 月 30 日开发者内部出现矛盾之后的四个月中，并无活跃开发者参与维护，其他组织成员也并未有足够贡献量/能力，因此我们决定停止维护本项目，最新 Release 可见于 [v2.7.0](https://github.com/Plumbum/Plumbum/releases/tag/v2.7.0)。

感谢所有 Plumbum 用户，项目组成员与外部贡献者，V2Fly 项目组成员及 V2Ray-Core 外部贡献者

致敬

gcc 写于 8 月 17 日

-----

## Availability & CI/CD Status

[![Snap Status](https://img.shields.io/travis/com/Plumbum/Plumbum?label=snapcraft-travis&logo=github)](https://travis-ci.com/Plumbum/Plumbum)
[![Plumbum build debian package](https://github.com/Plumbum/Plumbum/actions/workflows/deb.yml/badge.svg)](https://github.com/Plumbum/Plumbum/actions/workflows/deb.yml)
[![Plumbum Windows Installer](https://github.com/Plumbum/Plumbum/actions/workflows/nsis.yml/badge.svg)](https://github.com/Plumbum/Plumbum/actions/workflows/nsis.yml)
[![Plumbum build matrix - cmake](https://github.com/Plumbum/Plumbum/actions/workflows/build-plumbum-cmake.yml/badge.svg)](https://github.com/Plumbum/Plumbum/actions/workflows/build-plumbum-cmake.yml)
[![Plumbum build matrix - Qt6](https://github.com/Plumbum/Plumbum/actions/workflows/build-plumbum-qt6.yml/badge.svg)](https://github.com/Plumbum/Plumbum/actions/workflows/build-plumbum-qt6.yml)

<table>
 <tr>
  <td>ArchLinuxCN</td>
  <td>
   <a href="https://build.archlinuxcn.org/packages/#/plumbum">
    <img alt="ArchlinuxCN Stable" src="https://img.shields.io/badge/dynamic/json?label=archlinuxcn-stable&query=%24.latest.pkgver&url=https%3A%2F%2Fbuild.archlinuxcn.org%2Fapi%2Fpackages%2Fplumbum" />
   </a><br />

   <a href="https://build.archlinuxcn.org/packages/#/plumbum-dev-git">
    <img alt="ArchlinuxCN Development" src="https://img.shields.io/badge/dynamic/json?label=archlinuxcn-dev-git&query=%24.latest.pkgver&url=https%3A%2F%2Fbuild.archlinuxcn.org%2Fapi%2Fpackages%2Fplumbum-dev-git" />
   </a>
  </td>
 </tr>

 <tr>
 <td>AUR</td>
 <td>
   <a href="https://aur.archlinux.org/packages/plumbum">
    <img alt="AUR Stable" src="https://img.shields.io/aur/version/plumbum?label=aur-stable" />
   </a><br/>

   <a href="https://aur.archlinux.org/packages/plumbum-dev-git">
    <img alt="AUR Development" src="https://img.shields.io/aur/version/plumbum-dev-git?label=aur-development" /></a>
 </td>
 </tr>

 <tr>
  <td>Fedora</td>
  <td>
   <a href="https://build.opensuse.org/package/show/home:zzndb:Plumbum/Plumbum">
    <img alt="OBS Stable" src="https://img.shields.io/badge/dynamic/xml?color=3c6eb4&label=OBS%20stable&query=substring-before%28substring-after%28%2F%2F%40filename%5Bcontains%28.%2C%20%27src.rpm%27%29%5D%2C%20%27Plumbum-%27%29%2C%20%27-%27%29&url=https%3A%2F%2Fapi.opensuse.org%2Fpublic%2Fbuild%2Fhome%3Azzndb%3APlumbum%2FFedora_Rawhide%2Fx86_64%2FPlumbum" />
   </a><br/>
   <a href="https://build.opensuse.org/package/show/home:zzndb:Plumbum/Plumbum-preview">
    <img alt="OBS Preview" src="https://img.shields.io/badge/dynamic/xml?color=3c6eb4&label=OBS%20preview&query=substring-before%28substring-after%28%2F%2F%40filename%5Bcontains%28.%2C%20%27src.rpm%27%29%5D%2C%20%27Plumbum-preview-%27%29%2C%20%27-%27%29&url=https%3A%2F%2Fapi.opensuse.org%2Fpublic%2Fbuild%2Fhome%3Azzndb%3APlumbum%2FFedora_Rawhide%2Fx86_64%2FPlumbum-preview" />
   </a>
  </td>
 </tr>

 <tr>
  <td>Flathub</td>
  <td><a href="https://flathub.org/apps/details/com.github.Plumbum"><img alt="flathub" src="https://img.shields.io/badge/flathub-available-success" /></a></td>
 </tr>

 <tr>
  <td>Scoop</td>
  <td><a href="https://github.com/lukesampson/scoop-extras/blob/master/bucket/plumbum.json"><img alt="scoop" src="https://img.shields.io/badge/scoop--extras-available-blue" /></a></td>
 </tr>

 <tr>
 <td>Chocolatey</td>
 <td><a href="https://chocolatey.org/packages/plumbum"><img alt="Chocolatey Version" src="https://img.shields.io/chocolatey/v/plumbum"></a></td>
 </tr>

 <tr>
  <td>Snapcraft</td>
  <td>
   <a href="https://snapcraft.io/plumbum/"><img alt="snap" src="https://snapcraft.io/plumbum/badge.svg" /></a>
  </td>
 </tr>

 <tr>
  <td>openSUSE</td>
  <td>
   <a href="https://build.opensuse.org/package/show/home:zzndb:Plumbum/Plumbum">
    <img alt="OBS Stable" src="https://img.shields.io/badge/dynamic/xml?color=73ba25&label=OBS%20stable&query=substring-before%28substring-after%28%2F%2F%40filename%5Bcontains%28.%2C%20%27src.rpm%27%29%5D%2C%20%27Plumbum-%27%29%2C%20%27-%27%29&url=https%3A%2F%2Fapi.opensuse.org%2Fpublic%2Fbuild%2Fhome%3Azzndb%3APlumbum%2FopenSUSE_Tumbleweed%2Fx86_64%2FPlumbum" />
   </a><br/>
   <a href="https://build.opensuse.org/package/show/home:zzndb:Plumbum/Plumbum-preview">
    <img alt="OBS Stable" src="https://img.shields.io/badge/dynamic/xml?color=73ba25&label=OBS%20preview&query=substring-before%28substring-after%28%2F%2F%40filename%5Bcontains%28.%2C%20%27src.rpm%27%29%5D%2C%20%27Plumbum-preview-%27%29%2C%20%27-%27%29&url=https%3A%2F%2Fapi.opensuse.org%2Fpublic%2Fbuild%2Fhome%3Azzndb%3APlumbum%2FopenSUSE_Tumbleweed%2Fx86_64%2FPlumbum-preview" />
   </a>
  </td>
 </tr>
</table>

Translation Platform: **[Crowdin](https://crowdin.com/project/plumbum)**

## Special Thanks

**[JetBrains Developer Toolbox](https://www.jetbrains.com/?from=Plumbum)**

-------------------------------

## Licences

This is free software, and you are welcome to redistribute it under certain conditions.

### Third-party Libraries and Resources
See: [assets/credit.html](assets/credit.html)

Plumbum is licenced under [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

```
    Plumbum, A Qt frontend for V2Ray. Written in C++

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
## Star History

![stars](https://starchart.cc/Plumbum/Plumbum.svg)
