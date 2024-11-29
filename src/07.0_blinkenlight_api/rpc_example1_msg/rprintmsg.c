/*
 * rprintmsg.c: remote version of "printmsg.c"
 */
#include <stdio.h>
#include <rpc/rpc.h> /* always needed */
#include "msgrpc.h" /* need this too: will be generated by rpcgen */
main(argc, argv)
	int argc;char *argv[];
{
	CLIENT *cl;
	int *result;
	char *server;
	int	count ;
	my2params params ;
	char *message;
	if (argc < 4)
	{
		fprintf(stderr, "usage: %s host message count\n", argv[0]);
		exit(1);
	}
	/*
	 * Save values of command line arguments
	 */
	server = argv[1];
	message = argv[2];
	count = atoi(argv[3]) ;
	/*
	 * Create client "handle" used for calling MESSAGEPROG on the
	 * server designated on the command line. We tell the RPC package
	 * to use the "tcp" protocol when contacting the server.
	 */
	cl = clnt_create(server, MESSAGEPROG, MESSAGEVERS, "tcp");
	if (cl == NULL)
	{
		/*
		 * Couldn't establish connection with server.
		 * Print error message and die.
		 */
		clnt_pcreateerror(server);
		exit(1);
	}
	/*
	 * Call the remote procedure "printmessage" on the server
	 */
	params.message = message ;
	params.counter = count ;
	result = printmessage_1(&params, cl);
	if (result == NULL)
	{
		/*
		 * An error occurred while calling the server.
		 * Print error message and die.
		 */
		clnt_perror(cl, server);
		exit(1);
	}
	/*
	 * Okay, we successfully called the remote procedure.
	 */
	if (*result == 0)
	{
		/*
		 * Server was unable to print our message.
		 * Print error message and die.
		 */
		fprintf(stderr, "%s: %s couldn't print your message\n", argv[0], server);
		exit(1);
	}
	/*
	 * The message got printed on the server�s console
	 */
	printf("Message delivered to %s!\n", server);
}
