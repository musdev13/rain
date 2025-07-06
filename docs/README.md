<h1 align="center">🌧️Rain - Simple TUI player🎵</h1>
<p align="center" style="border: 3px solid white; border-radius:15px;padding: 15px 0 8px;">
    <a href="https://t.me/musdev" style="margin-right:5px;">
        <img src="https://img.shields.io/badge/MusDev--2CA5E0?logo=telegram&style=for-the-badge" alt="Telegram" style="border-radius:5px;border:3px solid #2ca5e0"/>
    </a>
    <a href="https://discord.com/users/870324942166716487" style="margin-right:5px;">
        <img src="https://img.shields.io/badge/cuteemus--5865F2?logo=discord&style=for-the-badge" alt="Discord" style="border-radius:5px;border:3px solid #5865f2"/>
    </a>
    <a href="https://github.com/musdev13">
        <img src="https://img.shields.io/badge/musdev13--181717?logo=github&style=for-the-badge" style="border-radius:5px;border:3px solid #181717">
    </a>
</p>

<img src="public/assets/mainScreenshot.png" style="border-radius:15px;border:3px solid white;margin-bottom:10px;">

<div align="center">

> It's just my C++ thesis💻, so it's not even beta, I guess. Rain is still in development and may contain a bunch of bugs🐛 So... don't worry about that. Just report me about them, k.? :3
</div>

<div align="center", style="border: 3px solid white; border-radius:15px;padding:10px 20px">
    <h1 align="center">About</h1>
    <p><b>Rain🌧️ — is a TUI player based on mpv</b>. It can play locally downloaded tracks in a folder, but tracks from Spotify🎧 and later SoundCloud☁️ will be added soon!</p>
    <p>And... of course, it's absolutely free!💖</p>


<h2 align="center">What can it do?</h2>
<p align="center">in fact, just the <b>basic things</b>:</p>

<div align="center">

| Feauture             |done?|                                                  descibtion |
|:---------------------|:---:|------------------------------------------------------------:|
| Play/Pause track     | ✅  |                                             It was not hard |
| Change track volume  | ✅  |                                It's very usefull for me btw |
| Change track pos     | ✅  |                   Idk, it's not hard to made like first two |
| Search track         | ✅  |             It was SO hard to make this at the first moment |
| Spotify tracks       | 🛠️  | I made a functions, but I haven't used them in the code yet |
| SoundCloud tracks    | ❌  |                               It will be in the soon future |
| Youtube Music tracks | ❌  |                                     Yeah, it's a great idea |
| Offline playback     | ❌  |     I need to make Spotify and SoundCloud tracks work first |

</div>
</div>

<div align="center" style="border: 3px solid white; border-radius:15px;padding:10px 20px;margin-top:15px">
<h1>Build</h1>

> Yeah,Rain does not have a binary version yet, but if you really want, I can upload Rain on [AUR](https://wiki.archlinux.org/title/Arch_User_Repository) or [GitHub Releases](https://github.com/musdev13/rain/releases)

<h2>Dependencies</h2>
<p>I use arch btw, so i can just leave a <b>list of packages</b> you need to install at this moment:</p>

<i>P.S.:It's not a names of packages that pacman use</i>

<ul style="list-style-position: inside; text-align: center; padding-left: 0; font-weight: bold;">
    <li>g++</li>
    <li>make</li>
    <li><a href="https://github.com/ArthurSonzogni/FTXUI">ftxui</a></li>
    <li>libmpv</li>
    <li>tagLib</li>
</ul>

> Tell me pls if i forgot to add some library to list

<h2>Actually a building process</h2>

first you need to **clone repo** and **cd into Rain directory**:
```sh
git clone --depth 1 https://github.com/musdev13/rain.git
cd rain
```
Now you can **build Rain using make**:
```sh
make
```
Now you can find **rain binary in the build folder**

---
> you can install rain binary manually into /usr/bin if you want using cp or mv and chmod
```sh
sudo mv build/rain /usr/bin/
chmod +x /usr/bin/rain
```

</div>

<div align="center" style="border: 3px solid white; border-radius:15px;padding:10px 20px;margin-top:15px">

<h1>How to use</h1>

You need to **run rain** and tell the **path of folder** that contain your tracks

**this path will be your playlist**

>**Warning: folder must contain only your tracks and nothing else**

```sh
rain /path/to/playlist/folder
```

---
**Rain will create a playlist file** in the folder, which will **contain a list of the tracks** in the folder.

You **can change the track order** to your own **in this file**.

> changes will be applied after restart

If you later add the track to a folder, Rain will **automatically add the file to the list** on startup

<h2>Navigation</h2>

on startup, **a panel that explains the hotkeys will open on the right side** of the window

**here's the hotkeys:**

| hotkey |                       action |
|:-------|-----------------------------:|
| /      |          focus on search bar |
| Ctrl+S | focus on pause/resume button |
| Ctrl+↔ |        change track position |
| Ctrl+B |         show/hide help panel |
| Ctrl+Q |                         quit |

</div>
