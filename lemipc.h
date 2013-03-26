lemipc/main.c                                                                                       0000644 0001750 0001750 00000005000 12124371300 014135  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #include	"lemipc.h"

void		lock(struct sembuf *sops)
{
  sops->sem_num = 0;
  sops->sem_flg = 0;
  sops->sem_op = -1;
}

void		unlock(struct sembuf *sops)
{
  sops->sem_num = 0;
  sops->sem_flg = 0;
  sops->sem_op = 1;
}

int		get_rand_number()
{
  return (rand() % 50);
}

int		count_team(char *map, char c)
{
  int		count;
  int		i;

  i = 0;
  count = 0;
  while (map[i] != '\0')
    {
      if (map[i] == c)
	count++;
      i++;
    }
  return (count);
}

int		is_quitting(char *addr, char team, int pos)
{
  return (0);
}

int		main(int ac, char **av)
{
  key_t		key;
  int		shm_id;
  int		sem_id;
  char		*addr;
  struct sembuf sops;
  
  if (ac != 2)
    {
      printf("Usage : %s pathname\n", av[0]);
      return (-1);
    }
  srand(time(NULL));
  key = ftok(av[1], 0);
  printf("Key : %d\n", key);
  shm_id = shmget(key, 42, SHM_R | SHM_W);
  printf("shmId = %d\n", shm_id);
  if (shm_id == -1)
    {
      sem_id = semget(key, 1, SHM_R | SHM_W | IPC_CREAT);
      shm_id = shmget(key,  42, IPC_CREAT | SHM_R | SHM_W);
      addr = shmat(shm_id, NULL, SHM_R | SHM_W);
      sprintf((char *)addr, "ooooooooooooooooooooooooooooooooooooooooooooo");
      printf("Created shm segment %d\n", shm_id);
      semctl(sem_id, 0, SETVAL, 1);
      while (1)
	{
	  printf("%s\n", addr);
	  lock(&sops);
	  semctl(sem_id, 0, GETVAL);
	  semop(sem_id, &sops, 1);
	  addr[5] = '1';
	  unlock(&sops);
	  semop(sem_id, &sops, 1);
	  semctl(sem_id, 0, GETVAL);
	  sleep(1);
	}
    }
  else
    {
      char	team;
      int	access;
      int	new;
      int	ret;

      access = 0;
      addr = shmat(shm_id, NULL, SHM_R | SHM_W);
      ret = 0;
      while (1 && ret == 0)
	{
	  lock(&sops);
	  semctl(sem_id, 0, GETVAL);
	  semop(sem_id, &sops, 1);
	  if (access == 0)
	    {
	      access++;
	      if (count_team(addr, '1') > count_team(addr, '2'))
		team = '2';
	      else
		team = '1';
	      printf("etape 1 team %c\n", team);
	    }
	  else if (access == 1)
	    {
	      access++;
	      new = get_rand_number();
	      while (addr[new] != 'o')
		new = get_rand_number();
	      addr[new] = team;
	      printf("etape 2\n");
	    }
	  else
	    {
	      if (is_quitting(addr, team, new) == 1)
		ret = -1;
	      else
		{
		  addr[new] = 'o';
		  new = get_rand_number();
		  while (addr[new] != 'o')
		    new = get_rand_number();
		  addr[new] = team;
		  printf("etape 3 position %d\n", new);
		}
	    }
	  unlock(&sops);
	  semop(sem_id, &sops, 1);
	  semctl(sem_id, 0, GETVAL);
	  sleep(1);
	}
      shmctl(shm_id, IPC_RMID, NULL);
    }
  return (0);
}
lemipc/Makefile                                                                                     0000644 0001750 0001750 00000000257 12124057226 014526  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              NAME	=	lemipc

SRC	=	main.c\

OBJ	=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
		cc -o $(NAME) $(OBJ)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re:		fclean all                                                                                                                                                                                                                                                                                                                                                 lemipc/                                                                                             0000755 0001750 0001750 00000000000 12124371617 013065  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/Makefile                                                                                     0000644 0001750 0001750 00000000000 12124057226 017420  1lemipc/Makefile                                                                                     ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/main.c                                                                                       0000644 0001750 0001750 00000000000 12124371300 016470  1lemipc/main.c                                                                                       ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/                                                                                        0000755 0001750 0001750 00000000000 12124372074 013724  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/hooks/                                                                                  0000755 0001750 0001750 00000000000 12124121545 015042  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/hooks/pre-applypatch.sample                                                             0000755 0001750 0001750 00000000616 12124121545 021204  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to verify what is about to be committed
# by applypatch from an e-mail message.
#
# The hook should exit with non-zero status after issuing an
# appropriate message if it wants to stop the commit.
#
# To enable this hook, rename this file to "pre-applypatch".

. git-sh-setup
test -x "$GIT_DIR/hooks/pre-commit" &&
	exec "$GIT_DIR/hooks/pre-commit" ${1+"$@"}
:
                                                                                                                  lemipc/.git/hooks/pre-rebase.sample                                                                 0000755 0001750 0001750 00000011442 12124121545 020277  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# Copyright (c) 2006, 2008 Junio C Hamano
#
# The "pre-rebase" hook is run just before "git rebase" starts doing
# its job, and can prevent the command from running by exiting with
# non-zero status.
#
# The hook is called with the following parameters:
#
# $1 -- the upstream the series was forked from.
# $2 -- the branch being rebased (or empty when rebasing the current branch).
#
# This sample shows how to prevent topic branches that are already
# merged to 'next' branch from getting rebased, because allowing it
# would result in rebasing already published history.

publish=next
basebranch="$1"
if test "$#" = 2
then
	topic="refs/heads/$2"
else
	topic=`git symbolic-ref HEAD` ||
	exit 0 ;# we do not interrupt rebasing detached HEAD
fi

case "$topic" in
refs/heads/??/*)
	;;
*)
	exit 0 ;# we do not interrupt others.
	;;
esac

# Now we are dealing with a topic branch being rebased
# on top of master.  Is it OK to rebase it?

# Does the topic really exist?
git show-ref -q "$topic" || {
	echo >&2 "No such branch $topic"
	exit 1
}

# Is topic fully merged to master?
not_in_master=`git rev-list --pretty=oneline ^master "$topic"`
if test -z "$not_in_master"
then
	echo >&2 "$topic is fully merged to master; better remove it."
	exit 1 ;# we could allow it, but there is no point.
fi

# Is topic ever merged to next?  If so you should not be rebasing it.
only_next_1=`git rev-list ^master "^$topic" ${publish} | sort`
only_next_2=`git rev-list ^master           ${publish} | sort`
if test "$only_next_1" = "$only_next_2"
then
	not_in_topic=`git rev-list "^$topic" master`
	if test -z "$not_in_topic"
	then
		echo >&2 "$topic is already up-to-date with master"
		exit 1 ;# we could allow it, but there is no point.
	else
		exit 0
	fi
else
	not_in_next=`git rev-list --pretty=oneline ^${publish} "$topic"`
	/usr/bin/perl -e '
		my $topic = $ARGV[0];
		my $msg = "* $topic has commits already merged to public branch:\n";
		my (%not_in_next) = map {
			/^([0-9a-f]+) /;
			($1 => 1);
		} split(/\n/, $ARGV[1]);
		for my $elem (map {
				/^([0-9a-f]+) (.*)$/;
				[$1 => $2];
			} split(/\n/, $ARGV[2])) {
			if (!exists $not_in_next{$elem->[0]}) {
				if ($msg) {
					print STDERR $msg;
					undef $msg;
				}
				print STDERR " $elem->[1]\n";
			}
		}
	' "$topic" "$not_in_next" "$not_in_master"
	exit 1
fi

<<\DOC_END

This sample hook safeguards topic branches that have been
published from being rewound.

The workflow assumed here is:

 * Once a topic branch forks from "master", "master" is never
   merged into it again (either directly or indirectly).

 * Once a topic branch is fully cooked and merged into "master",
   it is deleted.  If you need to build on top of it to correct
   earlier mistakes, a new topic branch is created by forking at
   the tip of the "master".  This is not strictly necessary, but
   it makes it easier to keep your history simple.

 * Whenever you need to test or publish your changes to topic
   branches, merge them into "next" branch.

The script, being an example, hardcodes the publish branch name
to be "next", but it is trivial to make it configurable via
$GIT_DIR/config mechanism.

With this workflow, you would want to know:

(1) ... if a topic branch has ever been merged to "next".  Young
    topic branches can have stupid mistakes you would rather
    clean up before publishing, and things that have not been
    merged into other branches can be easily rebased without
    affecting other people.  But once it is published, you would
    not want to rewind it.

(2) ... if a topic branch has been fully merged to "master".
    Then you can delete it.  More importantly, you should not
    build on top of it -- other people may already want to
    change things related to the topic as patches against your
    "master", so if you need further changes, it is better to
    fork the topic (perhaps with the same name) afresh from the
    tip of "master".

Let's look at this example:

		   o---o---o---o---o---o---o---o---o---o "next"
		  /       /           /           /
		 /   a---a---b A     /           /
		/   /               /           /
	       /   /   c---c---c---c B         /
	      /   /   /             \         /
	     /   /   /   b---b C     \       /
	    /   /   /   /             \     /
    ---o---o---o---o---o---o---o---o---o---o---o "master"


A, B and C are topic branches.

 * A has one fix since it was merged up to "next".

 * B has finished.  It has been fully merged up to "master" and "next",
   and is ready to be deleted.

 * C has not merged to "next" at all.

We would want to allow C to be rebased, refuse A, and encourage
B to be deleted.

To compute (1):

	git rev-list ^master ^topic next
	git rev-list ^master        next

	if these match, topic has not merged in next at all.

To compute (2):

	git rev-list master..topic

	if this is empty, it is fully merged to "master".

DOC_END
                                                                                                                                                                                                                              lemipc/.git/hooks/update.sample                                                                     0000755 0001750 0001750 00000007033 12124121545 017535  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to blocks unannotated tags from entering.
# Called by "git receive-pack" with arguments: refname sha1-old sha1-new
#
# To enable this hook, rename this file to "update".
#
# Config
# ------
# hooks.allowunannotated
#   This boolean sets whether unannotated tags will be allowed into the
#   repository.  By default they won't be.
# hooks.allowdeletetag
#   This boolean sets whether deleting tags will be allowed in the
#   repository.  By default they won't be.
# hooks.allowmodifytag
#   This boolean sets whether a tag may be modified after creation. By default
#   it won't be.
# hooks.allowdeletebranch
#   This boolean sets whether deleting branches will be allowed in the
#   repository.  By default they won't be.
# hooks.denycreatebranch
#   This boolean sets whether remotely creating branches will be denied
#   in the repository.  By default this is allowed.
#

# --- Command line
refname="$1"
oldrev="$2"
newrev="$3"

# --- Safety check
if [ -z "$GIT_DIR" ]; then
	echo "Don't run this script from the command line." >&2
	echo " (if you want, you could supply GIT_DIR then run" >&2
	echo "  $0 <ref> <oldrev> <newrev>)" >&2
	exit 1
fi

if [ -z "$refname" -o -z "$oldrev" -o -z "$newrev" ]; then
	echo "Usage: $0 <ref> <oldrev> <newrev>" >&2
	exit 1
fi

# --- Config
allowunannotated=$(git config --bool hooks.allowunannotated)
allowdeletebranch=$(git config --bool hooks.allowdeletebranch)
denycreatebranch=$(git config --bool hooks.denycreatebranch)
allowdeletetag=$(git config --bool hooks.allowdeletetag)
allowmodifytag=$(git config --bool hooks.allowmodifytag)

# check for no description
projectdesc=$(sed -e '1q' "$GIT_DIR/description")
case "$projectdesc" in
"Unnamed repository"* | "")
	echo "*** Project description file hasn't been set" >&2
	exit 1
	;;
esac

# --- Check types
# if $newrev is 0000...0000, it's a commit to delete a ref.
zero="0000000000000000000000000000000000000000"
if [ "$newrev" = "$zero" ]; then
	newrev_type=delete
else
	newrev_type=$(git cat-file -t $newrev)
fi

case "$refname","$newrev_type" in
	refs/tags/*,commit)
		# un-annotated tag
		short_refname=${refname##refs/tags/}
		if [ "$allowunannotated" != "true" ]; then
			echo "*** The un-annotated tag, $short_refname, is not allowed in this repository" >&2
			echo "*** Use 'git tag [ -a | -s ]' for tags you want to propagate." >&2
			exit 1
		fi
		;;
	refs/tags/*,delete)
		# delete tag
		if [ "$allowdeletetag" != "true" ]; then
			echo "*** Deleting a tag is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/tags/*,tag)
		# annotated tag
		if [ "$allowmodifytag" != "true" ] && git rev-parse $refname > /dev/null 2>&1
		then
			echo "*** Tag '$refname' already exists." >&2
			echo "*** Modifying a tag is not allowed in this repository." >&2
			exit 1
		fi
		;;
	refs/heads/*,commit)
		# branch
		if [ "$oldrev" = "$zero" -a "$denycreatebranch" = "true" ]; then
			echo "*** Creating a branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/heads/*,delete)
		# delete branch
		if [ "$allowdeletebranch" != "true" ]; then
			echo "*** Deleting a branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	refs/remotes/*,commit)
		# tracking branch
		;;
	refs/remotes/*,delete)
		# delete tracking branch
		if [ "$allowdeletebranch" != "true" ]; then
			echo "*** Deleting a tracking branch is not allowed in this repository" >&2
			exit 1
		fi
		;;
	*)
		# Anything else (is there anything else?)
		echo "*** Update hook: unknown type of update to ref $refname of type $newrev_type" >&2
		exit 1
		;;
esac

# --- Finished
exit 0
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     lemipc/.git/hooks/post-update.sample                                                                0000755 0001750 0001750 00000000275 12124121545 020521  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to prepare a packed repository for use over
# dumb transports.
#
# To enable this hook, rename this file to "post-update".

exec git update-server-info
                                                                                                                                                                                                                                                                                                                                   lemipc/.git/hooks/commit-msg.sample                                                                 0000755 0001750 0001750 00000001600 12124121545 020321  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to check the commit log message.
# Called by "git commit" with one argument, the name of the file
# that has the commit message.  The hook should exit with non-zero
# status after issuing an appropriate message if it wants to stop the
# commit.  The hook is allowed to edit the commit message file.
#
# To enable this hook, rename this file to "commit-msg".

# Uncomment the below to add a Signed-off-by line to the message.
# Doing this in a hook is a bad idea in general, but the prepare-commit-msg
# hook is more suited to it.
#
# SOB=$(git var GIT_AUTHOR_IDENT | sed -n 's/^\(.*>\).*$/Signed-off-by: \1/p')
# grep -qs "^$SOB" "$1" || echo "$SOB" >> "$1"

# This example catches duplicate Signed-off-by lines.

test "" = "$(grep '^Signed-off-by: ' "$1" |
	 sort | uniq -c | sed -e '/^[ 	]*1[ 	]/d')" || {
	echo >&2 Duplicate Signed-off-by lines.
	exit 1
}
                                                                                                                                lemipc/.git/hooks/pre-commit.sample                                                                 0000755 0001750 0001750 00000003250 12124121545 020324  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to verify what is about to be committed.
# Called by "git commit" with no arguments.  The hook should
# exit with non-zero status after issuing an appropriate message if
# it wants to stop the commit.
#
# To enable this hook, rename this file to "pre-commit".

if git rev-parse --verify HEAD >/dev/null 2>&1
then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

# If you want to allow non-ascii filenames set this variable to true.
allownonascii=$(git config hooks.allownonascii)

# Redirect output to stderr.
exec 1>&2

# Cross platform projects tend to avoid non-ascii filenames; prevent
# them from being added to the repository. We exploit the fact that the
# printable range starts at the space character and ends with tilde.
if [ "$allownonascii" != "true" ] &&
	# Note that the use of brackets around a tr range is ok here, (it's
	# even required, for portability to Solaris 10's /usr/bin/tr), since
	# the square bracket bytes happen to fall in the designated range.
	test $(git diff --cached --name-only --diff-filter=A -z $against |
	  LC_ALL=C tr -d '[ -~]\0' | wc -c) != 0
then
	echo "Error: Attempt to add a non-ascii file name."
	echo
	echo "This can cause problems if you want to work"
	echo "with people on other platforms."
	echo
	echo "To be portable it is advisable to rename the file ..."
	echo
	echo "If you know what you are doing you can disable this"
	echo "check using:"
	echo
	echo "  git config hooks.allownonascii true"
	echo
	exit 1
fi

# If there are whitespace errors, print the offending file names and fail.
exec git diff-index --check --cached $against --
                                                                                                                                                                                                                                                                                                                                                        lemipc/.git/hooks/prepare-commit-msg.sample                                                         0000755 0001750 0001750 00000002327 12124121545 021764  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to prepare the commit log message.
# Called by "git commit" with the name of the file that has the
# commit message, followed by the description of the commit
# message's source.  The hook's purpose is to edit the commit
# message file.  If the hook fails with a non-zero status,
# the commit is aborted.
#
# To enable this hook, rename this file to "prepare-commit-msg".

# This hook includes three examples.  The first comments out the
# "Conflicts:" part of a merge commit.
#
# The second includes the output of "git diff --name-status -r"
# into the message, just before the "git status" output.  It is
# commented because it doesn't cope with --amend or with squashed
# commits.
#
# The third example adds a Signed-off-by line to the message, that can
# still be edited.  This is rarely a good idea.

case "$2,$3" in
  merge,)
    /usr/bin/perl -i.bak -ne 's/^/# /, s/^# #/#/ if /^Conflicts/ .. /#/; print' "$1" ;;

# ,|template,)
#   /usr/bin/perl -i.bak -pe '
#      print "\n" . `git diff --cached --name-status -r`
#	 if /^#/ && $first++ == 0' "$1" ;;

  *) ;;
esac

# SOB=$(git var GIT_AUTHOR_IDENT | sed -n 's/^\(.*>\).*$/Signed-off-by: \1/p')
# grep -qs "^$SOB" "$1" || echo "$SOB" >> "$1"
                                                                                                                                                                                                                                                                                                         lemipc/.git/hooks/applypatch-msg.sample                                                             0000755 0001750 0001750 00000000704 12124121545 021202  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              #!/bin/sh
#
# An example hook script to check the commit log message taken by
# applypatch from an e-mail message.
#
# The hook should exit with non-zero status after issuing an
# appropriate message if it wants to stop the commit.  The hook is
# allowed to edit the commit message file.
#
# To enable this hook, rename this file to "applypatch-msg".

. git-sh-setup
test -x "$GIT_DIR/hooks/commit-msg" &&
	exec "$GIT_DIR/hooks/commit-msg" ${1+"$@"}
:
                                                            lemipc/.git/refs/                                                                                   0000755 0001750 0001750 00000000000 12124121647 014661  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/refs/remotes/                                                                           0000755 0001750 0001750 00000000000 12124121647 016337  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/refs/remotes/origin/                                                                    0000755 0001750 0001750 00000000000 12124362406 017626  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/refs/remotes/origin/master                                                              0000644 0001750 0001750 00000000051 12124362406 021040  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              a936226d10640a8f7100c995987eb1b6a7ec0d2b
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       lemipc/.git/refs/heads/                                                                             0000755 0001750 0001750 00000000000 12124362376 015753  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/refs/heads/master                                                                       0000644 0001750 0001750 00000000051 12124362376 017165  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              a936226d10640a8f7100c995987eb1b6a7ec0d2b
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       lemipc/.git/refs/tags/                                                                              0000755 0001750 0001750 00000000000 12124121545 015614  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/branches/                                                                               0000755 0001750 0001750 00000000000 12124121545 015504  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/COMMIT_EDITMSG                                                                          0000644 0001750 0001750 00000000526 12124362376 016022  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              ok
# Please enter the commit message for your changes. Lines starting
# with '#' will be ignored, and an empty message aborts the commit.
#
# Committer: guillaume <guillaume@debian.local.localhost>
#
# On branch master
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#	modified:   lemipc.h
#	modified:   main.c
#
                                                                                                                                                                          lemipc/.git/description                                                                             0000644 0001750 0001750 00000000111 12124121545 016156  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              Unnamed repository; edit this file 'description' to name the repository.
                                                                                                                                                                                                                                                                                                                                                                                                                                                       lemipc/.git/config                                                                                  0000644 0001750 0001750 00000000411 12124121647 015106  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              [core]
	repositoryformatversion = 0
	filemode = true
	bare = false
	logallrefupdates = true
[remote "origin"]
	fetch = +refs/heads/*:refs/remotes/origin/*
	url = git@github.com:boufflGEpitech/lemipc.git
[branch "master"]
	remote = origin
	merge = refs/heads/master
                                                                                                                                                                                                                                                       lemipc/.git/HEAD                                                                                    0000644 0001750 0001750 00000000027 12124121545 014342  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              ref: refs/heads/master
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         lemipc/.git/index                                                                                   0000644 0001750 0001750 00000000406 12124362367 014762  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              DIRC      QP�w    QP^�       0F&  ��  �  �   �Z:g�n2����2�:cl�-�� Makefile  QP�[    QP�[       0Eg  ��  �  �   ݓ���ٮ[?��B�D��Esh lemipc.h  QQ��    QQ��       0EN  ��  �  �  �Vw�f�(ޓ�ת��e�e�Z main.c    TREE    -1 0
>0�A_�r��Kb                                                                                                                                                                                                                                                          lemipc/.git/logs/                                                                                   0000755 0001750 0001750 00000000000 12124121620 014655  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/logs/refs/                                                                              0000755 0001750 0001750 00000000000 12124121647 015625  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/logs/refs/remotes/                                                                      0000755 0001750 0001750 00000000000 12124121647 017303  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/logs/refs/remotes/origin/                                                               0000755 0001750 0001750 00000000000 12124121647 020572  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/logs/refs/remotes/origin/master                                                         0000644 0001750 0001750 00000000735 12124362406 022015  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              0000000000000000000000000000000000000000 3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f guillaume <guillaume@debian.local.localhost> 1364239271 -0400	update by push
3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f b086b0fde48dd58596ba9aa9bad460ce4b0ee15c guillaume <guillaume@debian.local.localhost> 1364248188 -0400	update by push
b086b0fde48dd58596ba9aa9bad460ce4b0ee15c a936226d10640a8f7100c995987eb1b6a7ec0d2b guillaume <guillaume@debian.local.localhost> 1364321542 -0400	update by push
                                   lemipc/.git/logs/refs/heads/                                                                        0000755 0001750 0001750 00000000000 12124121620 016700  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/logs/refs/heads/master                                                                  0000644 0001750 0001750 00000000733 12124362376 020140  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              0000000000000000000000000000000000000000 3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f guillaume <guillaume@debian.local.localhost> 1364239247 -0400	commit (initial): ok
3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f b086b0fde48dd58596ba9aa9bad460ce4b0ee15c guillaume <guillaume@debian.local.localhost> 1364248180 -0400	commit: ok
b086b0fde48dd58596ba9aa9bad460ce4b0ee15c a936226d10640a8f7100c995987eb1b6a7ec0d2b guillaume <guillaume@debian.local.localhost> 1364321532 -0400	commit: ok
                                     lemipc/.git/logs/HEAD                                                                               0000644 0001750 0001750 00000000733 12124362376 015323  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              0000000000000000000000000000000000000000 3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f guillaume <guillaume@debian.local.localhost> 1364239247 -0400	commit (initial): ok
3e6ab7d4af3d71fd3bc69741d128037f7a5f2e3f b086b0fde48dd58596ba9aa9bad460ce4b0ee15c guillaume <guillaume@debian.local.localhost> 1364248180 -0400	commit: ok
b086b0fde48dd58596ba9aa9bad460ce4b0ee15c a936226d10640a8f7100c995987eb1b6a7ec0d2b guillaume <guillaume@debian.local.localhost> 1364321532 -0400	commit: ok
                                     lemipc/.git/objects/                                                                                0000755 0001750 0001750 00000000000 12124362376 015361  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/aa/                                                                             0000755 0001750 0001750 00000000000 12124143162 015730  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/aa/835754653ed3d2a999d6dfabeca25412189b4a                                       0000444 0001750 0001750 00000001073 12124143162 023074  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x�UMo�@�j���Q�m��M�P�
*R�B����NL�%{�B�;3k;�C�>���yo����Q�������0����8̼����a"5-�ab�p����9|��W:����~��H���	��P4�e��6ྟ�l!�ғP��-(Ҭ +�`rzc��8X�7@���i�|.�5��q�Hx,�'��6��=��e��yjզG��@a�LL����lC���W|�&+E�ß��&��l���83���_;<��X�'�؀��X��R&��ǀBv��=o�Iv��	VE{�i�LfW��u���	�d���`�x.�p|��Nw���\�i��ݬZ�O����O��v�\p)|�V`�㺼U�T��sFLL��ҵM2N
T��6bЁZ���$e��v�A4�ۘfh;�*O{22� ������r9��E�f����dZxw&�cg�	���8,?"�Z�^ڶ �Q!:;`_��4���9�zU>AH]��Ψ��6�A%���.ro~�q�6nw����øh����͡N=����À��������.jZ������ Oo                                                                                                                                                                                                                                                                                                                                                                                                                                                                     lemipc/.git/objects/pack/                                                                           0000755 0001750 0001750 00000000000 12124121545 016266  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/b0/                                                                             0000755 0001750 0001750 00000000000 12124143165 015653  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/b0/86b0fde48dd58596ba9aa9bad460ce4b0ee15c                                       0000444 0001750 0001750 00000000233 12124143165 023366  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x��A� E]s
.�a
41ƫ�`i1��_��p��6��s[��k��o"��H@G(x�≬d+r*�����M֮Q<�����`b?l4J��X��h�S��m�k�}}��5K�i=��T�;�g�h@﬋�>g����.�vT���O	                                                                                                                                                                                                                                                                                                                                                                     lemipc/.git/objects/56/                                                                             0000755 0001750 0001750 00000000000 12124362367 015613  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/56/77de66c528de930b139dd7aac1e565b165d25a                                       0000444 0001750 0001750 00000001065 12124362367 023032  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x�T�n�@������t9(���"U������CY{[�M���gf�v�T ~�wgfϙ93�U���9wΞ��R/�}eX�J��{Z���"�0��ۈ�*j��R%�:�a����ܙ ��A���	\�x�3���1��6r����ĢI��2�DF��H�/��r��ب�.������2L��?l�B����1��_p�=eH�J Bz��&����H뮔k�ᤄ\Va*��Z6���x9 z
U�E
b�4�=:0Q�0����hgiØ�-��#��x�N]���ZU�6�Ml����]��~��`����F����f�K�y�� .:*�	�f�lquy˘Tn��K���b�L��Yj�.OVMr6|���>">�5z	=�����!mW�o,��mìP�R>ՂJ����XJ.[%^�v3���r���m?��I������Q�@`��i<��g/��N)���11"ko�wY�1��H���%
~�rts���O��s�T�� 0< �U\*޴W�;~��_���zL��|U�u����.-n�o��7�A���E�͟k(��                                                                                                                                                                                                                                                                                                                                                                                                                                                                           lemipc/.git/objects/33/                                                                             0000755 0001750 0001750 00000000000 12124362375 015605  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/33/e13613aa72e0a4780201820d8b36f8e239f215                                       0000444 0001750 0001750 00000000164 12124362375 022420  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x+)JMU040c040031Q�M�NM��Ie��J��g�oǜGF��D�s���	U����Y����0yu㎛���2r�s��r�kqTMnbf�^2CX�����&sϽ����ԍ��� �-�                                                                                                                                                                                                                                                                                                                                                                                                            lemipc/.git/objects/26/                                                                             0000755 0001750 0001750 00000000000 12124143162 015576  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/26/eedcfa31e4bd9c9ce6cd73f055429239633185                                       0000444 0001750 0001750 00000000157 12124143162 023027  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              xK��OR0��dP�L�KIM�䌏�q��p�����R
e楢�r)g�%甦�r����d&�eء�e�e楣�U�T�c
g$c
��b� rq)��d�q ��B[                                                                                                                                                                                                                                                                                                                                                                                                                 lemipc/.git/objects/cf/                                                                             0000755 0001750 0001750 00000000000 12124143164 015741  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/cf/ee73c991fd1c5cf6aa2ed2e81dbf510a86a061                                       0000444 0001750 0001750 00000000165 12124143164 023365  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x+)JMU040c040031Q�M�NM��Ie��J��g�oǜGF��D�s���	U����Y��������/�'{��yv��C��$�d�V�����<�d�U��!�v�/��y���7�B�$f{ ��.�                                                                                                                                                                                                                                                                                                                                                                                                           lemipc/.git/objects/3e/                                                                             0000755 0001750 0001750 00000000000 12124121620 015651  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/3e/6ab7d4af3d71fd3bc69741d128037f7a5f2e3f                                       0000444 0001750 0001750 00000000174 12124121620 023150  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x���! ��
�,�<.�[Y`�#�$'�o���3����[+C�E���,	�ό���6Zevd)$�@A����^�{?�}�Zi6��Oo�c���D�˽��U*�P�E��g@ �>���vD�7�@?                                                                                                                                                                                                                                                                                                                                                                                                    lemipc/.git/objects/a4/                                                                             0000755 0001750 0001750 00000000000 12124121617 015654  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/a4/27de40db2ffa98ade6a5a8c0b0a82352615447                                       0000444 0001750 0001750 00000000164 12124121617 023055  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x+)JMU040c040031Q�M�NM��Ie��J��g�oǜGF��D�s���	U����Y����`9[.v���.�=�UG�UV����M���Kf0OY��B�t�Ϸ7>������ de-�                                                                                                                                                                                                                                                                                                                                                                                                            lemipc/.git/objects/39/                                                                             0000755 0001750 0001750 00000000000 12124121615 015601  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/39/9b1e5dbc18f78a38e88c0f687ac43b25e17bef                                       0000444 0001750 0001750 00000000150 12124121615 023167  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              xK��OR04�`P�L�KIM�䌏�q��p�����R
e楢�r)g�%甦�r����d&�eء�e�	U�T�c
g$c
��b� r)��d�q y;�                                                                                                                                                                                                                                                                                                                                                                                                                        lemipc/.git/objects/5a/                                                                             0000755 0001750 0001750 00000000000 12124121615 015653  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/5a/3a67956e32feb89ce232933a15636cad2dcef9                                       0000444 0001750 0001750 00000000204 12124121615 023075  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              xK��OR047e�s�u���I��,H��
r�r3���c������\���^��^�&WbN�'P�ȅ2��"@՚\����
��
Pq �J�IM̳J�*���� �@] y$i��E�@� ��6 �U,,                                                                                                                                                                                                                                                                                                                                                                                            lemipc/.git/objects/37/                                                                             0000755 0001750 0001750 00000000000 12124121615 015577  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/37/64aef4d0771b83f9edd8f33c4c734e2dcaf924                                       0000444 0001750 0001750 00000000465 12124121615 023172  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x��OO�@Ž�O�1)d5�ы��A�`L�xP�,��6�i�|wg�6�'��;;o~�f��f�����"Ua����f����:q'U���e�<~A�a"t:r�v~`M�K�rd�e�i4tL��'C���os�s�KW�;/eL��e��ԉ�9})W@�?���A�S��
�U��:p�[b��ެ=V�]���Ϝgpdy,�ɣ7��GL��������,��k�+5�k�ܺ1@�i�,i�:����{EkR�8��qARSd()ΉW\�=�O������2/S���Q�:�&0.���A�e>��4�wm�yp� �}��                                                                                                                                                                                                           lemipc/.git/objects/a9/                                                                             0000755 0001750 0001750 00000000000 12124362376 015672  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/a9/36226d10640a8f7100c995987eb1b6a7ec0d2b                                       0000444 0001750 0001750 00000000233 12124362376 022653  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              x��Q� D��{��������6R1���F/��d>&o^n�2w0}"��51F��РKH�� �r4ک'���0��cJq�E�82���z�b�h�o}j+ܶ�V���^�����2�oN��/��X����-���.�rT��7x�M�                                                                                                                                                                                                                                                                                                                                                                     lemipc/.git/objects/93/                                                                             0000755 0001750 0001750 00000000000 12124362367 015614  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/objects/93/ab81b8d9ae5b3f9d0108de42d644d8e3457368                                       0000444 0001750 0001750 00000000166 12124362367 022764  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              xmʱ
�0 P��+���ť-����4����:6v}<�.۶)Y6h� x���p��Tc�R�~5(ni1�t�_���L��lK��ELW�8^1��]�8}�3�W*dCV��=J1                                                                                                                                                                                                                                                                                                                                                                                                          lemipc/.git/objects/info/                                                                           0000755 0001750 0001750 00000000000 12124121545 016303  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/info/                                                                                   0000755 0001750 0001750 00000000000 12124121545 014652  5                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              lemipc/.git/info/exclude                                                                            0000644 0001750 0001750 00000000360 12124121545 016225  0                                                                                                    ustar   guillaume                       guillaume                                                                                                                                                                                                              # git ls-files --others --exclude-from=.git/info/exclude
# Lines that start with '#' are comments.
# For a project mostly in C, the following would be a good set of
# exclude patterns (uncomment them if you want to use them):
# *.[oa]
# *~
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                