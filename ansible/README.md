# About

Scripts here are for use with [Ansible][6]. 

Ansible is a configuration management tool that can make it easier to update multiple RecalBox machines at once, which can be a significant time savings in a classroom environment.

Ansible must be run on a separate computer on the same network as the RecalBox machines to be updated.

Ansible is only official supported running on a Linux machine, but it is possible to [run Ansible inside Windows][7].

Note that this only simplifies the setup. A Linux-based RecalBox machine is still required for each target machine.

[6]: https://www.ansible.com/resources/get-started
[7]: https://docs.ansible.com/ansible/latest/user_guide/windows_faq.html#can-ansible-run-on-windows

## Setup

Install Ansible.

```shell
pip3 install ansible
```

Set up SSH login to your RecalBox machine(s).

```shell
ssh-keygen
ssh-copy-id root@recalbox.local
```

## Install McAirPos to your RecalBox machine(s)

```shell
$ ansible-playbook ./ansible_install.yml --inventory 'asteroids.local,rampage.local' --user 'root'

PLAY [Install McAirPos to allow playing MakeCode Arcade games] ***************************************

... various task details ...


PLAY RECAP *******************************************************************************************
asteroids.local              : ok=4    changed=2    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
rampage.local              : ok=4    changed=2    unreachable=0    failed=0    skipped=0    rescued=0    ignored=0
```

## Optional

For long lists of hosts, add all RecalBox machines to a group named `arcades` in the inventory file `/etc/ansible/hosts`.

```ini
[arcades]
asteroids.local	ansible_user=root
rampage.local	ansible_user=root
```

Then run the playbook with a simpler command:
```shell
ansible-playbook ./ansible_install.yml
```