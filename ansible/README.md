# About

Scripts here are for use with [Ansible][6]. 

Ansible is a configuration management tool that can make it easier to update multiple Recalbox machines at once, which can be a significant time saving in a classroom environment.

Ansible must be run on a separate _host_ computer on the same network as the Recalbox machines to be updated.

Ansible is only official supported for running on a Linux _host_ computer, but it is possible to [run Ansible inside Windows][7].

An ARM Linux-based Recalbox machine is required for each _target_ machine.

[6]: https://www.ansible.com/resources/get-started
[7]: https://docs.ansible.com/ansible/latest/user_guide/windows_faq.html#can-ansible-run-on-windows

## Setup

Install Ansible on the _host_ computer.

```shell
pip3 install ansible
```
OR
```shell
sudo apt update && sudo apt upgrade && sudo apt install ansible -y
```
Set up SSH login to your Recalbox _target_ machine(s).

```shell
ssh-keygen
ssh-copy-id root@recalbox.local
```

## Install McAirPos on your _target_ Recalbox machine(s)

```shell
$ ansible-playbook -i "rampage.local," --user "root" ./ansible_install.yml

PLAY [Install McAirPos to allow playing MakeCode Arcade games] ***************************************

... various task details ...


PLAY RECAP *******************************************************************************************
rampage.local              : ok=4    changed=2    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
```

## Optional

For long lists of _targets_, add all Recalbox machines to a group named `arcades` in the inventory file `/etc/ansible/hosts`.

```ini
[arcades]
asteroids.local	ansible_user=root
rampage.local	ansible_user=root
```

Then run the playbook with a simpler command:
```shell
ansible-playbook -l 'arcades,' ./ansible_install.yml
```
