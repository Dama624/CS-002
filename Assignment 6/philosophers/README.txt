In the greedy algorithm, each philosopher first picks up their left fork,
then picks up their right fork. The deadlock occurs after each picks up their
left fork; when all philosophers' left forks are picked up, there are no more
forks left for any of them to pick up. As a result, none of them can pick up
a right fork, and so they deadlock.

The maximum number of philosophers that can sit at the table and guarantee
no deadlock is the Number of Philosophers - 1. With one less philosopher, we
have NUM forks for (NUM - 1) philosophers. As a result, we can guarantee that
at least one philosopher will have access to two forks, and for as long as
that philosopher puts down the forks after eating, there will not be a
deadlock.

Consider the case in which there was no philosopher numbering requirement.
All philosophers begin with dirty left forks. All philosophers
want to eat; none of them have a dirty right fork, and so they all request
one. All philosophers (since they are all holding left forks) receive a
request message. Since all forks are dirty, the philosophers all clean the
forks and send the forks over. However, since all philosophers had one fork
and all of them requested another, they effectively all exchanged forks, which
are now clean. We repeat the process, but every philosopher is holding a clean
fork, and the philosophers keep clean forks. Thus, a deadlock occurs.