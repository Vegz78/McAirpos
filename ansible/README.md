# About

Scripts here are for use with Ansible.

Ansible runs best on a Linux system. 

On Windows, consider following these instruction under [Windows Subsystem for Linux][6].

[6]: https://docs.microsoft.com/en-us/windows/wsl/install

## Setup

Install Ansible.

```
pip3 install ansible
```

Set up SSH login to your RecalBox machine(s).

```
ssh-keygen
ssh-copy-id root@recalbox.local
```

Add your RecalBox machines to a group named `arcades` 
in `/etc/ansible/hosts`

```
[arcades]
asteroids.local	ansible_user=root
rampgage.local	ansible_user=root
```

## Install McAirPos to your RecalBox machine(s)

```sh
$ ansible-playbook ansible_install.yml

PLAY [Install McAirPos to allow playing MakeCode Arcade games] ***************************************

... various task details ...


PLAY RECAP *******************************************************************************************
rampage.local              : ok=4    changed=2    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
```
