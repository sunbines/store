#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <rados/librados.h>

int main(int argc, char *argv[])
{
	int ret;
	rados_t clnt;
	rados_write_op_t op = NULL;
	rados_ioctx_t io_ctx;
	char *pool = "test";
	char *ns = "test";
	char *oid = "grace";

	uint64_t cur = htole64(1); // starting epoch
	uint64_t rec = htole64(0); // no recovery yet
	char buf[sizeof(uint64_t) * 2];

	ret = rados_create(&clnt, NULL);
	if (ret < 0)
	{
		fprintf(stderr, "rados_create: %d\n", ret);
		return ret;
	}

	ret = rados_conf_read_file(clnt, "/etc/ceph/ceph.conf");
	if (ret < 0)
	{
		fprintf(stderr, "rados_conf_read_file: %d\n", ret);
		return ret;
	}

	ret = rados_connect(clnt);
	if (ret < 0)
	{
		fprintf(stderr, "rados_connect: %d\n", ret);
		return ret;
	}

	ret = rados_pool_create(clnt, pool);
	if (ret < 0 && ret != -17)
	{
		fprintf(stderr, "rados_pool_create: %d\n", ret);
		return ret;
	}

	ret = rados_ioctx_create(clnt, pool, &io_ctx);
	if (ret < 0)
	{
		fprintf(stderr, "rados_ioctx_create: %d\n", ret);
		return ret;
	}

	rados_ioctx_set_namespace(io_ctx, ns);

	memcpy(buf, (char *)&cur, sizeof(cur));
	memcpy(buf + sizeof(cur), (char *)&rec, sizeof(rec));

	op = rados_create_write_op();
	/* Create the object */
	rados_write_op_create(op, LIBRADOS_CREATE_EXCLUSIVE, NULL);

	/* Set serial numbers if we created the object */
	rados_write_op_write_full(op, buf, sizeof(buf));

	ret = rados_write_op_operate(op, io_ctx, oid, NULL, 0);
	rados_release_write_op(op);

	return ret;
}