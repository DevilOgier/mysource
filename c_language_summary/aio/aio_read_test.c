#ifdef __cplusplus
extern "C"{
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <malloc.h>

#include <fcntl.h>
#include <libaio.h>

#ifndef O_DIRECT
/* savannah bug #20050, #20052 */
#define O_DIRECT 0 /* From asm/fcntl.h */
#endif


// ������� warning: implicit declaration of function ��posix_memalign��
#ifdef USE_XOPEN2K
/* Allocate memory of SIZE bytes with an alignment of ALIGNMENT.  */
extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __THROW __nonnull ((1));
#endif

int main(int argc, char *argv[])
{
	// ÿ�ζ���32K�ֽ�  32768
	const int buffer_size = 0x8000;

	// ����¼��� 32
	const int nr_events   = 32;
	int rt;

	io_context_t ctx = {0};

	// ��ʹ�� io_context_t
	//ϵͳ����io_setup�ᴴ��һ����ν��"AIO������"(��aio_context������Ҳ�С�AIO context����)�ṹ�嵽���ں���
	rt = io_setup(nr_events, &ctx);
	if ( rt != 0 )
		error(1, rt, "io_setup");

	// ���ζ�ȡ������Ϊ�ļ��������ύ��ctx
	// ��ȡϵͳҳ��С
	int pagesize = sysconf(_SC_PAGESIZE);
	for (int i=1; i<argc; ++i) {
		struct iocb *cb = (struct iocb*)malloc(sizeof(struct iocb));
		void *buffer;
		// Ҫʹ��O_DIRECT, ����Ҫ���룬ҳ��С����
		posix_memalign(&buffer, pagesize, buffer_size);
        //��Ҫ�����ò���
		io_prep_pread(cb, open(argv[i], O_RDONLY | O_DIRECT), buffer, buffer_size, 0);
        //
		rt = io_submit(ctx, 1, &cb);
		if (rt < 0)
			error(1, -rt, "io_submit %s", argv[i]);;
	}

	struct  io_event events[nr_events];
	struct  iocb     *cbs[nr_events];
    struct  io_event event;

	int remain = argc - 1;
	int n      = 0;

	// ����������С���ص�������Ϊ1�����Ϊnr_events
	// io_getevents() returns the number of events read
	while (remain && (n = io_getevents(ctx, 1, nr_events, events, 0))) {
		int nr_cbs = 0;
		for (int i=0; i<n; ++i) {
			event = events[i];
			struct  iocb     *cb    = event.obj;
			// event.resΪunsigned
			//printf("%d receive %d bytes\n", cb->aio_fildes, event.res);
			if (event.res > buffer_size) {
				printf("%s\n", strerror(-event.res));
			}
            
            //������
			if (event.res != buffer_size || event.res2 != 0) {
				--remain;
				// �ͷ�buffer, fd �� cb
				free(cb->u.c.buf);
				close(cb->aio_fildes);
				free(cb);
            //��û����
			} else {
				// ����cb��offset
				cb->u.c.offset += event.res;
				cbs[nr_cbs++] = cb;
			}
		}

		if (nr_cbs) {
			// ������������
			io_submit(ctx, nr_cbs, cbs);
		}
	}
    //io_destroy���������������������aio_context_t
    rt = io_destroy(ctx);
    if (rt < 0) {
        perror("io_destroy error");
        return -1;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif